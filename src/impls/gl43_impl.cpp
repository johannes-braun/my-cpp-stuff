#include <impls/gl43_impl.hpp>
#include <processing/sift/sift.hpp>
#include <processing/image.hpp>
#include <fstream>
#include <string>
#include <opengl/mygl_glfw.hpp>
#include <iostream>
#include <imgui/imgui.h>

namespace mpp
{
    namespace
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

        constexpr auto screen_vert_src = R"(#version 330 core
out vec2 vs_uv;
void main()
{
    gl_Position = vec4(mix(-1.f, 3.f, float(gl_VertexID & 0x1)), mix(-1.f, 3.f, float((gl_VertexID >> 1) & 0x1)), 0.f, 1.f);
    vs_uv = ((gl_Position.xy+1)*0.5f);
}
)";

        constexpr auto texture_frag_scr = R"(#version 330 core
in vec2 vs_uv;
layout(location=0) out vec4 color;
uniform sampler2D in_texture;
void main()
{
    color = texture(in_texture, vec2(vs_uv.x, 1-vs_uv.y));
}
)";
    }

    constexpr auto points_vert_src = R"(#version 330 core
layout(location=0) in vec2 in_pos;
void main()
{
    gl_Position = vec4(in_pos, 0, 1);
}
)";
    constexpr auto simple_color_frag_src = R"(#version 330 core
layout(location=0) out vec4 color;
void main()
{
    color = vec4(1, 0, 0, 1);
}
)";

    std::uint32_t allocate_textures(GLenum format, bool gen_mipmap, const image& img)
    {
        std::vector<std::uint32_t> tex(1);
        glGenTextures(int(tex.size()), tex.data());

        for (auto id : tex)
        {
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            GLenum swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, reinterpret_cast<int*>(swizzleMask));
            glTexImage2D(GL_TEXTURE_2D, 0, format, img.dimensions().x, img.dimensions().y, 0, GL_RED, GL_UNSIGNED_BYTE, img.data());
            if (gen_mipmap)
                glGenerateMipmap(GL_TEXTURE_2D);
        }

        return tex[0];
    }

    void gl43_impl::on_setup(program_state& state)
    {
        glfwWindowHint(GLFW_SAMPLES, 4);
    }
    void gl43_impl::on_start(program_state& state)
    {
        glEnable(GL_MULTISAMPLE);
        const auto screen_vert = create_shader(GL_VERTEX_SHADER, screen_vert_src);
        const auto texture_frag = create_shader(GL_FRAGMENT_SHADER, texture_frag_scr);
        full_screen.program = create_program(texture_frag, screen_vert);
        glDeleteShader(screen_vert);
        glDeleteShader(texture_frag);
        full_screen.in_texture_location = glGetUniformLocation(full_screen.program, "in_texture");

        const auto points_vert = create_shader(GL_VERTEX_SHADER, points_vert_src);
        const auto simple_color_frag = create_shader(GL_FRAGMENT_SHADER, simple_color_frag_src);
        points.program = create_program(simple_color_frag, points_vert);
        glDeleteShader(points_vert);
        glDeleteShader(simple_color_frag);

        glGenVertexArrays(1, &points.vao);
        glBindVertexArray(points.vao);
        glGenBuffers(1, &points.vbo);
        glGenBuffers(1, &points.ori_vbo);
        glEnableVertexAttribArray(0);

        add_img("../../res/IMG_20190605_174704.jpg");
        add_img("../../res/IMG_20190605_174705.jpg");
        add_img("../../res/bun.jpg");
        add_img("../../res/arch.jpg");
        add_img("../../res/butterfly.jpg");
    }
    void gl43_impl::on_update(program_state& state, seconds delta)
    {
        glDisable(GL_DEPTH_TEST);
        glUseProgram(full_screen.program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[current_texture]);
        glUniform1i(full_screen.in_texture_location, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(points.vao);
        glUseProgram(points.program);
        glBindBuffer(GL_ARRAY_BUFFER, points.vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(sift::feature), nullptr);
        glPointSize(point_size);
        glBufferData(GL_ARRAY_BUFFER, features[current_texture].size() * sizeof(sift::feature), features[current_texture].data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, features[current_texture].size());

        glBindBuffer(GL_ARRAY_BUFFER, points.ori_vbo);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);
        glBufferData(GL_ARRAY_BUFFER, orientation_dbg[current_texture].size() * sizeof(glm::vec2), orientation_dbg[current_texture].data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINES, 0, orientation_dbg[current_texture].size());

        if (ImGui::Begin("Settings"))
        {
            ImGui::DragInt("Texture", &current_texture, 0.01f, 0, textures.size()-1);
            ImGui::DragFloat("Point Size", &point_size, 0.01f, 1.f, 100.f);
            ImGui::End();
        }
    }
    void gl43_impl::on_end(program_state& state)
    {
        glDeleteProgram(full_screen.program);
        glDeleteProgram(points.program);
        glDeleteVertexArrays(1, &points.vao);
        glDeleteBuffers(1, &points.vbo);
        glDeleteTextures(int(textures.size()), textures.data());
    }
    void gl43_impl::add_img(const char* path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::in);
        img.load_stream(file, 1);
        auto& ori = orientation_dbg.emplace_back();
        for (auto& feat : features.emplace_back(sift::sift(img, 4, 3)))
        {
            feat.x = (feat.x / img.dimensions().x) * 2.f - 1.f;
            feat.y = -((feat.y / img.dimensions().y) * 2.f - 1.f);

            ori.emplace_back(glm::vec2(feat.x, feat.y));
            ori.emplace_back(glm::vec2(feat.x, feat.y) + 0.05f*glm::vec2(glm::cos(feat.orientation), glm::sin(feat.orientation)));
        }
        textures.emplace_back(allocate_textures(GL_RED, true, img));
    }
}