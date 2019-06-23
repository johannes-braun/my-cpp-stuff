#include <platform/opengl.hpp>
#include <opengl/mygl_glfw.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>
#include <spdlog/spdlog.h>

namespace mpp
{
    void opengl_environment::on_setup(program_state& state) 
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    }

    void opengl_environment::on_start(program_state& state) 
    {
        mygl::load(reinterpret_cast<mygl::loader_function>(glfwGetProcAddress));
        ImGui_ImplOpenGL3_Init();

        spdlog::info("OpenGL Environment");

        int maj, min;
        glGetIntegerv(GL_MAJOR_VERSION, &maj);
        glGetIntegerv(GL_MINOR_VERSION, &min);
        spdlog::info("    GL Version: {}.{}", maj, min);
        spdlog::info("    Vendor: {}", glGetString(GL_VENDOR));
        spdlog::info("    GLSL Version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback([](GLenum source, GLenum type, std::uint32_t id, GLenum severity, std::int32_t length, const char* message, const void* userParam) {
            switch (type)
            {
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                spdlog::warn("OpenGL Deprecated: {}", message);
                break;
            case GL_DEBUG_TYPE_ERROR:
                spdlog::error("OpenGL Error: {}", message);
                break;
            case GL_DEBUG_TYPE_MARKER:
                spdlog::info("OpenGL Marker: {}", message);
                break;
            case GL_DEBUG_TYPE_OTHER:
                spdlog::debug("OpenGL Other: {}", message);
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                spdlog::warn("OpenGL Performance: {}", message);
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                spdlog::warn("OpenGL Undefined Behavior: {}", message);
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                spdlog::warn("OpenGL Portability: {}", message);
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                spdlog::debug("OpenGL Push Group: {}", message);
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                spdlog::debug("OpenGL Push Group: {}", message);
                break;
            }
            }, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);
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