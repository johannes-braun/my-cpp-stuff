#include "default_impl.hpp"
#include <opengl/mygl_glfw.hpp>
#include <program_state.hpp>
#include <iostream>
#include <glm/ext.hpp>

namespace mpp
{
    default_impl::~default_impl() {}
    void default_impl::on_setup(program_state& state)
    {
        glfwWindowHint(GLFW_SAMPLES, 8);
    }
    void default_impl::on_start(program_state& state) {
        _clear_color = glm::vec3(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX));
        glClearColor(_clear_color.x, _clear_color.y, _clear_color.z, 1.f);

        glm::vec3 verts[]{
            glm::vec3(0.5f, -0.5f, 0.f),
            glm::vec3(-0.5f, -0.5f, 0.f),
            glm::vec3(0.f, 1.f, 0.f)
        };
        glCreateBuffers(1, &_vbo);
        glNamedBufferStorage(_vbo, std::size(verts) * sizeof(glm::vec3), verts, GLenum{});
        glCreateVertexArrays(1, &_vao);
        glEnableVertexArrayAttrib(_vao, 0);
        glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, false, 0);
        glVertexArrayAttribBinding(_vao, 0, 0);

        auto vs = glCreateShader(GL_VERTEX_SHADER);
        constexpr auto vs_src = R"(#version 460 core
layout(location = 0) in vec3 in_pos;
layout(location = 0) uniform mat4 view_proj;
layout(location = 0) out vec3 vs_pos;
void main() {
    gl_Position = view_proj * vec4(in_pos, 1);
    vs_pos = in_pos;
}
)";
        glShaderSource(vs, 1, &vs_src, nullptr);
        glCompileShader(vs);
        int did_compile = 0;
        int info_log_length = 0;
        std::string info_log;

        glGetShaderiv(vs, GL_COMPILE_STATUS, &did_compile);
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &info_log_length);
        info_log.resize(info_log_length, '\0');
        glGetShaderInfoLog(vs, info_log_length, &info_log_length, info_log.data());
        std::cout << "Shader Compile Status:\n";
        std::cout << info_log << '\n';
        if (!did_compile)
            throw std::runtime_error("Did not compile");

        auto fs = glCreateShader(GL_FRAGMENT_SHADER);
        constexpr auto fs_src = R"(#version 460 core
layout(location = 0) in vec3 vs_pos;
layout(location = 0) out vec4 fs_color;
void main()
{
    fs_color = vec4(vs_pos, 1);
}
)";
        glShaderSource(fs, 1, &fs_src, nullptr);
        glCompileShader(fs);

        glGetShaderiv(fs, GL_COMPILE_STATUS, &did_compile);
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &info_log_length);
        info_log.resize(info_log_length, '\0');
        glGetShaderInfoLog(fs, info_log_length, &info_log_length, info_log.data());
        std::cout << "Shader Compile Status:\n";
        std::cout << info_log << '\n';
        if (!did_compile)
            throw std::runtime_error("Did not compile");

        _program = glCreateProgram();
        glAttachShader(_program, vs);
        glAttachShader(_program, fs);
        glLinkProgram(_program);

        glGetProgramiv(_program, GL_LINK_STATUS, &did_compile);
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &info_log_length);
        info_log.resize(info_log_length, '\0');
        glGetProgramInfoLog(_program, info_log_length, &info_log_length, info_log.data());
        std::cout << "Program Link Status:\n";
        std::cout << info_log << '\n';

        if (!did_compile)
            throw std::runtime_error("Did not link");

        glDetachShader(_program, vs);
        glDetachShader(_program, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        _camera.set_axis_smoothing(0.7f);
        _camera.set_rotate_smoothing(0.7f);
        _camera.set_position(glm::vec3(3, 3, 3));
        _camera.look_at(glm::vec3(0, 0, 0));
    }
    void default_impl::on_update(program_state & state, seconds delta) {

        const auto delta_millis = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(delta);
        const auto gcc = glfwGetCurrentContext();
        double cposx, cposy;
        glfwGetCursorPos(gcc, &cposx, &cposy);

        if (!ImGui::GetIO().WantCaptureKeyboard && !ImGui::GetIO().WantTextInput)
        {
            _camera.input_axis_x(float(glfwGetKey(gcc, GLFW_KEY_D) - glfwGetKey(gcc, GLFW_KEY_A)));
            _camera.input_axis_y(float(glfwGetKey(gcc, GLFW_KEY_E) - glfwGetKey(gcc, GLFW_KEY_Q)));
            _camera.input_axis_z(float(glfwGetKey(gcc, GLFW_KEY_W) - glfwGetKey(gcc, GLFW_KEY_S)));
            if (glfwGetKey(gcc, GLFW_KEY_LEFT_CONTROL))
                _camera.input_speed_factor(0.6f);
            else if(glfwGetKey(gcc, GLFW_KEY_LEFT_SHIFT))
                _camera.input_speed_factor(9.f);
            else
                _camera.input_speed_factor(3.f);
        }

        if (!ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(gcc, GLFW_MOUSE_BUTTON_LEFT))
        {
            _camera.input_rotate_h(float(cposx - _last_curpos.x));
            _camera.input_rotate_v(float(cposy - _last_curpos.y));

            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        _last_curpos = { cposx, cposy };
        _camera.update(delta_millis);
        glClear(GL_COLOR_BUFFER_BIT);

        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        const glm::mat4 view_proj =
            glm::perspectiveFov(glm::radians(60.f), float(viewport[2]), float(viewport[3]), 0.01f, 100.f) *
            _camera.view_matrix();
        glUseProgram(_program);
        glProgramUniformMatrix4fv(_program, 0, 1, false, value_ptr(view_proj));
        glBindVertexArray(_vao);
        glBindVertexBuffer(0, _vbo, 0, sizeof(glm::vec3));
        glDrawArrays(GL_TRIANGLES, 0, 3);

        if (ImGui::Begin("Whatever"))
        {
            char buf[256]{ 0 };
            ImGui::InputText("NoIdea", buf, 256);
            if (ImGui::Button("Go!"))
            {
                state.open_state(std::make_shared<default_impl>());
            }
            ImGui::End();
        }
    }
    void default_impl::on_end(program_state & state) {

    }
}