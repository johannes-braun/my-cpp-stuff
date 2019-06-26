#include <processing/sift/sift.hpp>
#include <processing/sift/detail/sift_state.hpp>
#include <processing/sift/detail/shaders.hpp>
#include <opengl/mygl.hpp>
#include <string>
#include <spdlog/spdlog.h>
#include "sift_state.hpp"

namespace mpp::sift::detail
{
    std::uint32_t create_shader(GLenum type, const char* src)
    {
        std::uint32_t sh = glCreateShader(type);
        glShaderSource(sh, 1, &src, nullptr);
        glCompileShader(sh);
        int log_len;
        glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 3)
        {
            std::string info_log(log_len, '\0');
            glGetShaderInfoLog(sh, log_len, &log_len, info_log.data());
            spdlog::info("Shader Compilation Output:\n{}", info_log);
        }
        return sh;
    }

    std::uint32_t create_program(std::uint32_t frag, std::uint32_t vert)
    {
        std::uint32_t program = glCreateProgram();
        glAttachShader(program, vert);
        glAttachShader(program, frag);
        glLinkProgram(program);
        int log_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 3)
        {
            std::string info_log(log_len, '\0');
            glGetProgramInfoLog(program, log_len, &log_len, info_log.data());
            spdlog::info("Shader Compilation Output:\n{}", info_log);
        }
        glDetachShader(program, vert);
        glDetachShader(program, frag);
        return program;
    }

    std::vector<std::uint32_t> allocate_textures(size_t count)
    {
        std::vector<std::uint32_t> tex(count);
        glGenTextures(int(tex.size()), tex.data());
        return tex;
    }

    void resize_textures(std::vector<std::uint32_t>& tex, int width, int height, GLenum format, int mips)
    {
        for (auto& id : tex)
        {
            if (glIsTexture(id))
                glDeleteTextures(1, &id);
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexStorage2D(GL_TEXTURE_2D, mips, format, width, height);
        }
    }

    sift_state::sift_state(size_t num_octaves, size_t num_feature_scales)
        : num_octaves(num_octaves), num_feature_scales(num_feature_scales)
    {
        // Allocate Textures needed for SIFT:
        // gaussian [r16f   ]: num_feature_scales + 2 outer + 1 extra
        // DoG      [r16f   ]: num_feature_scales + 2 outer
        // features [rgba16f]: num_feature_scales
        // temps    [r16f   ]: 2
        temporary_textures = allocate_textures(2);
        gaussian_textures = allocate_textures(num_feature_scales + 3);
        difference_of_gaussian_textures = allocate_textures(num_feature_scales + 2);
        feature_textures = allocate_textures(num_feature_scales);
        // Create Renderbuffers for stencil testing
        feature_stencil_buffers.resize(num_feature_scales * num_octaves);
        glGenRenderbuffers(int(feature_stencil_buffers.size()), feature_stencil_buffers.data());
        glGenBuffers(1, &filter_transform_buffer);
        glGenBuffers(1, &orientation_transform_buffer);
        glGenBuffers(1, &full_feature_buffer);

        // Shared Screen-Filling-Triangle Shader
        const auto screen_vert = create_shader(GL_VERTEX_SHADER, shader_source::screen_vert);
        const auto empty_fs = create_shader(GL_FRAGMENT_SHADER, "#version 320 es\n"
            "#if GL_ES\n"
            "precision highp float;\n"
            "#endif\n"
            "void main(){ }");

        // Create Gauss-Blur Program for DoG Pyramid Pre-Filtering
        {
            const auto gauss_fs = create_shader(GL_FRAGMENT_SHADER, shader_source::gauss_blur_frag);
            gauss_blur.program = create_program(gauss_fs, screen_vert);
            glDeleteShader(gauss_fs);
            gauss_blur.u_mip_location = glGetUniformLocation(gauss_blur.program, "u_mip");
            gauss_blur.u_dir_location = glGetUniformLocation(gauss_blur.program, "u_dir");
            gauss_blur.u_sigma_location = glGetUniformLocation(gauss_blur.program, "u_sigma");
            gauss_blur.in_texture_location = glGetUniformLocation(gauss_blur.program, "in_texture");
        }

        // Create Difference Program for DoG Pyramid Generation
        {
            const auto diff_fs = create_shader(GL_FRAGMENT_SHADER, shader_source::difference_frag);
            difference.program = create_program(diff_fs, screen_vert);
            glDeleteShader(diff_fs);
            difference.u_current_tex_location = glGetUniformLocation(difference.program, "u_current_tex");
            difference.u_previous_tex_location = glGetUniformLocation(difference.program, "u_previous_tex");
        }

        // Create Maximize Program for First Feature Selection
        {
            const auto max_fs = create_shader(GL_FRAGMENT_SHADER, shader_source::maximize_frag);
            maximize.program = create_program(max_fs, screen_vert);
            glDeleteShader(max_fs);
            maximize.u_previous_tex_location = glGetUniformLocation(maximize.program, "u_previous_tex");
            maximize.u_current_tex_location = glGetUniformLocation(maximize.program, "u_current_tex");
            maximize.u_next_tex_location = glGetUniformLocation(maximize.program, "u_next_tex");
            maximize.u_neighbors_location = glGetUniformLocation(maximize.program, "u_neighbors");
            maximize.u_mip_location = glGetUniformLocation(maximize.program, "u_mip");
            maximize.u_scale_location = glGetUniformLocation(maximize.program, "u_scale");
        }

        // Create Filter Program to remove outliers
        {
            const auto filter_gs = create_shader(GL_GEOMETRY_SHADER, shader_source::filter_geom);
            const auto filter_vs = create_shader(GL_VERTEX_SHADER, shader_source::filter_vert);

            filter.program = glCreateProgram();

            constexpr const char* tf_out_names[3]{ "feature", "octave", "pad3" };
            glTransformFeedbackVaryings(filter.program, int(std::size(tf_out_names)), std::data(tf_out_names), GL_INTERLEAVED_ATTRIBS);

            glAttachShader(filter.program, filter_vs);
            glAttachShader(filter.program, filter_gs);
            glAttachShader(filter.program, empty_fs);
            glLinkProgram(filter.program);
            int log_len;
            glGetProgramiv(filter.program, GL_INFO_LOG_LENGTH, &log_len);
            if (log_len > 3)
            {
                std::string info_log(log_len, '\0');
                glGetProgramInfoLog(filter.program, log_len, &log_len, info_log.data());
                spdlog::info("Shader Compilation Output:\n{}", info_log);
            }
            glDetachShader(filter.program, filter_vs);
            glDetachShader(filter.program, filter_gs);
            glDetachShader(filter.program, empty_fs);

            glDeleteShader(filter_vs);
            glDeleteShader(filter_gs);

            filter.u_previous_tex_location = glGetUniformLocation(filter.program, "u_previous_tex");
            filter.u_current_tex_location = glGetUniformLocation(filter.program, "u_current_tex");
            filter.u_next_tex_location = glGetUniformLocation(filter.program, "u_next_tex");
            filter.u_feature_tex_location = glGetUniformLocation(filter.program, "u_feature_tex");
            filter.u_scale_location = glGetUniformLocation(filter.program, "u_scale");
            filter.u_mip_location = glGetUniformLocation(filter.program, "u_mip");
            filter.u_border_location = glGetUniformLocation(filter.program, "u_border");
        }

        // Create Orientation Program
        {
            const auto orientation_gs = create_shader(GL_GEOMETRY_SHADER, shader_source::orientation_geom);
            const auto orientation_vs = create_shader(GL_VERTEX_SHADER, shader_source::orientation_vert);

            orientation.program = glCreateProgram();

            constexpr const char* tf_out_names[4] { "feature", "octave", "orientation", "pad2" };
            glTransformFeedbackVaryings(orientation.program, int(std::size(tf_out_names)), std::data(tf_out_names), GL_INTERLEAVED_ATTRIBS);

            glAttachShader(orientation.program, orientation_vs);
            glAttachShader(orientation.program, orientation_gs);
            glAttachShader(orientation.program, empty_fs);
            glLinkProgram(orientation.program);
            int log_len;
            glGetProgramiv(orientation.program, GL_INFO_LOG_LENGTH, &log_len);
            if (log_len > 3)
            {
                std::string info_log(log_len, '\0');
                glGetProgramInfoLog(orientation.program, log_len, &log_len, info_log.data());
                spdlog::info("Shader Compilation Output:\n{}", info_log);
            }
            glDetachShader(orientation.program, orientation_vs);
            glDetachShader(orientation.program, orientation_gs);
            glDetachShader(orientation.program, empty_fs);

            glDeleteShader(orientation_vs);
            glDeleteShader(orientation_gs);

            orientation.u_textures_locations[0] = glGetUniformLocation(orientation.program, "u_textures[0]");
            orientation.u_textures_locations[1] = glGetUniformLocation(orientation.program, "u_textures[1]");
            orientation.u_textures_locations[2] = glGetUniformLocation(orientation.program, "u_textures[2]");
            orientation.u_textures_locations[3] = glGetUniformLocation(orientation.program, "u_textures[3]");
            orientation.u_textures_locations[4] = glGetUniformLocation(orientation.program, "u_textures[4]");
            orientation.u_textures_locations[5] = glGetUniformLocation(orientation.program, "u_textures[5]");
            orientation.u_textures_locations[6] = glGetUniformLocation(orientation.program, "u_textures[6]");
            orientation.u_textures_locations[7] = glGetUniformLocation(orientation.program, "u_textures[7]");
            orientation.u_textures_locations[8] = glGetUniformLocation(orientation.program, "u_textures[8]");
            orientation.u_textures_locations[9] = glGetUniformLocation(orientation.program, "u_textures[9]");
            orientation.u_textures_locations[10] = glGetUniformLocation(orientation.program, "u_textures[10]");
            orientation.u_textures_locations[11] = glGetUniformLocation(orientation.program, "u_textures[11]");
            orientation.u_textures_locations[12] = glGetUniformLocation(orientation.program, "u_textures[12]");
            orientation.u_textures_locations[13] = glGetUniformLocation(orientation.program, "u_textures[13]");
            orientation.u_textures_locations[14] = glGetUniformLocation(orientation.program, "u_textures[14]");
            orientation.u_textures_locations[15] = glGetUniformLocation(orientation.program, "u_textures[15]");
        }

        {
            const auto descriptor_cs = create_shader(GL_COMPUTE_SHADER, shader_source::descriptor_comp);
            descriptor.program = glCreateProgram();
            glAttachShader(descriptor.program, descriptor_cs);
            glLinkProgram(descriptor.program);
            int log_len;
            glGetProgramiv(descriptor.program, GL_INFO_LOG_LENGTH, &log_len);
            if (log_len > 3)
            {
                std::string info_log(log_len, '\0');
                glGetProgramInfoLog(descriptor.program, log_len, &log_len, info_log.data());
                spdlog::info("Shader Compilation Output:\n{}", info_log);
            }
            glDetachShader(descriptor.program, descriptor_cs);

            glDeleteShader(descriptor_cs);

            descriptor.u_textures_locations[0] = glGetUniformLocation(descriptor.program, "u_textures[0]");
            descriptor.u_textures_locations[1] = glGetUniformLocation(descriptor.program, "u_textures[1]");
            descriptor.u_textures_locations[2] = glGetUniformLocation(descriptor.program, "u_textures[2]");
            descriptor.u_textures_locations[3] = glGetUniformLocation(descriptor.program, "u_textures[3]");
            descriptor.u_textures_locations[4] = glGetUniformLocation(descriptor.program, "u_textures[4]");
            descriptor.u_textures_locations[5] = glGetUniformLocation(descriptor.program, "u_textures[5]");
            descriptor.u_textures_locations[6] = glGetUniformLocation(descriptor.program, "u_textures[6]");
            descriptor.u_textures_locations[7] = glGetUniformLocation(descriptor.program, "u_textures[7]");
            descriptor.u_textures_locations[8] = glGetUniformLocation(descriptor.program, "u_textures[8]");
            descriptor.u_textures_locations[9] = glGetUniformLocation(descriptor.program, "u_textures[9]");
            descriptor.u_textures_locations[10] = glGetUniformLocation(descriptor.program, "u_textures[10]");
            descriptor.u_textures_locations[11] = glGetUniformLocation(descriptor.program, "u_textures[11]");
            descriptor.u_textures_locations[12] = glGetUniformLocation(descriptor.program, "u_textures[12]");
            descriptor.u_textures_locations[13] = glGetUniformLocation(descriptor.program, "u_textures[13]");
            descriptor.u_textures_locations[14] = glGetUniformLocation(descriptor.program, "u_textures[14]");
            descriptor.u_textures_locations[15] = glGetUniformLocation(descriptor.program, "u_textures[15]");
        }

        // Create transform feedback program for feature count reduction
        {
            const auto reduction_gs = create_shader(GL_GEOMETRY_SHADER, shader_source::transform_feedback_reduce_geom);
            const auto reduction_vs = create_shader(GL_VERTEX_SHADER, shader_source::transform_feedback_reduce_vert);
            const auto reduction_fs = empty_fs;

            transform_feedback_reduce.program = glCreateProgram();

            constexpr const char* tf_out_names[3]{ "feature_values", "octave", "skip_c" };
            glTransformFeedbackVaryings(transform_feedback_reduce.program, int(std::size(tf_out_names)), std::data(tf_out_names), GL_INTERLEAVED_ATTRIBS);

            glAttachShader(transform_feedback_reduce.program, reduction_vs);
            glAttachShader(transform_feedback_reduce.program, reduction_gs);
            glAttachShader(transform_feedback_reduce.program, reduction_fs);
            glLinkProgram(transform_feedback_reduce.program);
            int log_len;
            glGetProgramiv(transform_feedback_reduce.program, GL_INFO_LOG_LENGTH, &log_len);
            if (log_len > 3)
            {
                std::string info_log(log_len, '\0');
                glGetProgramInfoLog(transform_feedback_reduce.program, log_len, &log_len, info_log.data());
                spdlog::info("Shader Compilation Output:\n{}", info_log);
            }
            glDetachShader(transform_feedback_reduce.program, reduction_vs);
            glDetachShader(transform_feedback_reduce.program, reduction_gs);
            glDetachShader(transform_feedback_reduce.program, reduction_fs);

            glDeleteShader(reduction_gs);
            glDeleteShader(reduction_vs);

            transform_feedback_reduce.u_mip_location = glGetUniformLocation(transform_feedback_reduce.program, "u_mip");
            transform_feedback_reduce.u_texture_location = glGetUniformLocation(transform_feedback_reduce.program, "u_texture");
        }
        glDeleteShader(empty_fs);
        glDeleteShader(screen_vert);

        // Create one Framebuffer for each Octave (mip-level)
        framebuffers.resize(num_octaves);
        glGenFramebuffers(int(framebuffers.size()), framebuffers.data());

        // Create an empty vertex array to draw a screen-filling-triangle
        glGenVertexArrays(1, &empty_vao);
        glGenVertexArrays(1, &orientation_vao);
    }
    void sift_state::resize(int width, int height)
    {
        if (width == this->width && height == this->height)
            return;

        this->width = width;
        this->height = height;

        resize_textures(temporary_textures, width, height, GL_R32F, int(num_octaves));
        resize_textures(gaussian_textures, width, height, GL_R32F, int(num_octaves));
        resize_textures(difference_of_gaussian_textures, width, height, GL_R32F, int(num_octaves));
        resize_textures(feature_textures, width, height, GL_RGBA32F, int(num_octaves));

        for (int oct = 0; oct < num_octaves; ++oct)
        {
            for (int feat = 0; feat < num_feature_scales; ++feat)
            {
                glBindRenderbuffer(GL_RENDERBUFFER, feature_stencil_buffers[feat + oct * num_feature_scales]);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width >> oct, height >> oct);
            }
        }
    }
    sift_state::~sift_state()
    {
        glDeleteBuffers(1, &filter_transform_buffer);
        glDeleteBuffers(1, &orientation_transform_buffer);
        glDeleteBuffers(1, &full_feature_buffer);
        glDeleteVertexArrays(1, &empty_vao);
        glDeleteVertexArrays(1, &orientation_vao);
        glDeleteFramebuffers(int(framebuffers.size()), framebuffers.data());
        glDeleteTextures(int(temporary_textures.size()), temporary_textures.data());
        glDeleteTextures(int(gaussian_textures.size()), gaussian_textures.data());
        glDeleteTextures(int(difference_of_gaussian_textures.size()), difference_of_gaussian_textures.data());
        glDeleteTextures(int(feature_textures.size()), feature_textures.data());
        glDeleteRenderbuffers(int(feature_stencil_buffers.size()), feature_stencil_buffers.data());
        glDeleteProgram(gauss_blur.program);
        glDeleteProgram(difference.program);
        glDeleteProgram(maximize.program);
        glDeleteProgram(filter.program);
        glDeleteProgram(transform_feedback_reduce.program);
    }
}