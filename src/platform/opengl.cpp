#include <platform/opengl.hpp>
#include <opengl/mygl_glfw.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>

namespace mpp
{
    void opengl_environment::on_setup(program_state& state) 
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }

    void opengl_environment::on_start(program_state& state) 
    {
        mygl::read(reinterpret_cast<mygl::loader_function>(glfwGetProcAddress));
        ImGui_ImplOpenGL3_Init();

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback([](GLenum source, GLenum type, std::uint32_t id, GLenum severity, std::int32_t length, const char* message, const void* userParam) {
            std::cout << message << '\n';
            }, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
    }

    void opengl_environment::on_begin_update(program_state& state, seconds delta) {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void opengl_environment::on_end_update(program_state& state, seconds delta) {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(glfwGetCurrentContext());
    }

    void opengl_environment::on_end(program_state& state)
    {
    }
}