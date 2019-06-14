#include <processing/sift/sift.hpp>
#include <processing/sift/detail/sift_state.hpp>
#include <processing/sift/detail/shaders.hpp>
#include <opengl/mygl.hpp>
#include <string>
#include <iostream>

namespace mpp::sift::detail
{
    std::uint32_t create_shader(GLenum type, const char* src)
    {
        std::uint32_t sh = glCreateShader(type);
        glShaderSource(sh, 1, &src, nullptr);
        glCompileShader(sh);
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
        std::string info_log(log_len, '\0');
        glGetProgramInfoLog(program, log_len, &log_len, info_log.data());
        std::cout << info_log << '\n';
        glDetachShader(program, vert);
        glDetachShader(program, frag);
        return program;
    }

    std::vector<std::uint32_t> allocate_textures(size_t count, int width, int height, GLenum format, bool gen_mipmap)
    {
        std::vector<std::uint32_t> tex(count);
        glGenTextures(int(tex.size()), tex.data());

        for (auto id : tex)
        {
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            if(gen_mipmap)
                glGenerateMipmap(GL_TEXTURE_2D);
        }

        return tex;
    }

    sift_state::sift_state(size_t num_octaves, size_t num_feature_scales, int width, int height)
        : num_octaves(num_octaves), num_feature_scales(num_feature_scales)
    {
        // Shared Screen-Filling-Triangle Shader
        const auto screen_vert = create_shader(GL_VERTEX_SHADER, shader_source::screen_vert);

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
            const auto filter_fs = create_shader(GL_FRAGMENT_SHADER, shader_source::filter_frag);
            filter.program = create_program(filter_fs, screen_vert);
            glDeleteShader(filter_fs);
            filter.u_previous_tex_location = glGetUniformLocation(filter.program, "u_previous_tex");
            filter.u_current_tex_location = glGetUniformLocation(filter.program, "u_current_tex");
            filter.u_next_tex_location = glGetUniformLocation(filter.program, "u_next_tex");
            filter.u_feature_tex_location = glGetUniformLocation(filter.program, "u_feature_tex");
            filter.u_scale_location = glGetUniformLocation(filter.program, "u_scale");
            filter.u_mip_location = glGetUniformLocation(filter.program, "u_mip");
            filter.u_border_location = glGetUniformLocation(filter.program, "u_border");
        }

        // Create Orientation Program to compute the main feature orientations
        {
            const auto orientation_fs = create_shader(GL_FRAGMENT_SHADER, shader_source::orientation_frag);
            orientation.program = create_program(orientation_fs, screen_vert);
            glDeleteShader(orientation_fs);
            orientation.u_img_location = glGetUniformLocation(orientation.program, "u_images[0]");
            orientation.u_mip_location = glGetUniformLocation(orientation.u_mip_location, "u_mip");
            orientation.u_scale_location = glGetUniformLocation(orientation.program, "u_scale");
            orientation.u_orientation_slices_location = glGetUniformLocation(orientation.u_orientation_slices_location, "u_orientation_slices");
            orientation.u_orientation_magnitude_threshold_location = glGetUniformLocation(orientation.u_orientation_slices_location, "u_orientation_magnitude_threshold");
        }

        // Allocate Textures needed for SIFT:
        // gaussian [r32f   ]: num_feature_scales + 2 outer + 1 extra
        // DoG      [r32f   ]: num_feature_scales + 2 outer
        // features [rgba32f]: num_feature_scales
        // temps    [r32f   ]: 2
        temporary_textures = allocate_textures(2, width, height, GL_R32F, false);
        gaussian_textures = allocate_textures(num_feature_scales + 3, width, height, GL_R32F, false);
        difference_of_gaussian_textures = allocate_textures(num_feature_scales + 2, width, height, GL_R32F, false);
        feature_textures = allocate_textures(num_feature_scales, width, height, GL_RGBA32F, true);
        orientation_textures = allocate_textures(num_feature_scales, width, height, GL_R16F, true);

        // Create Renderbuffers for stencil testing
        feature_stencil_buffers.resize(num_feature_scales * num_octaves);
        glGenRenderbuffers(int(feature_stencil_buffers.size()), feature_stencil_buffers.data());
        for (int oct = 0; oct < num_octaves; ++oct)
        {
            for (int feat = 0; feat < num_feature_scales; ++feat)
            {
                glBindRenderbuffer(GL_RENDERBUFFER, feature_stencil_buffers[feat + oct * num_feature_scales]);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width >> oct, height >> oct);
            }
        }

        // Create one Framebuffer for each Octave (mip-level)
        framebuffers.resize(num_octaves);
        glGenFramebuffers(int(framebuffers.size()), framebuffers.data());

        // Create an empty vertex array to draw a screen-filling-triangle
        glGenVertexArrays(1, &empty_vao);
    }
    sift_state::~sift_state()
    {
        glDeleteVertexArrays(1, &empty_vao);
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
    }
}