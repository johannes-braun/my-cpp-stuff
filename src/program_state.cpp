#define GLFW_INCLUDE_NONE
#include <program_state.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <opengl/mygl.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

namespace mpp
{
    void program_state::poll()
    {
        while (_num_windows > 0)
        {
            {
                std::lock_guard lock(_state_mutex);
                if (!_to_remove.empty())
                {
                    for (auto& s : _to_remove)
                    {
                        auto it = _state_threads.find(s);
                        if (it->second.joinable())
                            it->second.join();
                        _state_threads.erase(it);
                    }
                    _to_remove.clear();
                }
            }
        }
        for (auto& p : _state_threads)
        {
            if (p.second.joinable())
                p.second.join();
        }
    }

    void program_state::open_state(std::shared_ptr<mpp::basic_visualization> state)
    {
        static struct glfw_init_t
        {
            glfw_init_t() { glfwInit(); }
            ~glfw_init_t() { glfwTerminate(); }
        } glfw_init;
        if (_state_threads.count(state) == 0)
        {
            ++_num_windows;
            std::lock_guard lock(_state_mutex);

            glfwDefaultWindowHints();
            state->on_setup(*this);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
            _state_threads.emplace(state, std::thread([=] {
                struct window_destructor
                {
                    void operator()(GLFWwindow* w) const {
                        glfwDestroyWindow(w);
                    }
                };
                struct imgui_destructor
                {
                    void operator()(ImGuiContext* ctx) const {
                        ImGui::DestroyContext(ctx);
                    }
                };
                std::unique_ptr<GLFWwindow, window_destructor> window{
                    glfwCreateWindow(1280, 720, "Visualization", nullptr, nullptr)
                };
                std::unique_ptr<ImGuiContext, imgui_destructor> imgui_context{
                    ImGui::CreateContext()
                };
                using time_point = std::chrono::steady_clock::time_point;
                using duration = time_point::duration;
                glfwMakeContextCurrent(window.get());
                mygl::load(reinterpret_cast<mygl::loader_function>(glfwGetProcAddress));

                ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
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

                state->on_start(*this);
                time_point last_checkmark = std::chrono::steady_clock::now();
                while (!glfwWindowShouldClose(window.get()))
                {
                    duration delta = std::chrono::steady_clock::now() - last_checkmark;
                    last_checkmark = std::chrono::steady_clock::now();
                    ImGui::SetCurrentContext(imgui_context.get());
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplGlfw_NewFrame();
                    ImGui::NewFrame();

                    state->on_update(*this, delta);

                    ImGui::Render();
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                    glfwSwapBuffers(window.get());
                    glfwPollEvents();
                }
                state->on_end(*this);
                --_num_windows;
                std::lock_guard lock(_state_mutex);
                _to_remove.emplace_back(state);
                }));
        }
    }
}