
#include <impls/menu_impl.hpp>
#include <imgui/imgui.h>
#include <program_state.hpp>
#include <opengl/mygl_glfw.hpp>

#include <impls/automaton_impl.hpp>
#include <impls/default_impl.hpp>
#include <impls/caves_impl.hpp>

namespace mpp
{
    menu_impl::~menu_impl()
    {
    }
    void menu_impl::on_start(program_state& state)
    {
        glfwSetWindowSize(glfwGetCurrentContext(), 300, 400);
    }

    namespace {
        template<typename Impl>
        void mk_button(program_state& state, const char* text)
        {
            if (ImGui::Button(text, ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
                state.open_state(std::make_shared<Impl>());
        }
    }

    void menu_impl::on_update(program_state& state, std::chrono::steady_clock::time_point::duration delta)
    {
        int win_width = 300;
        int win_height = 400;
        glfwGetWindowSize(glfwGetCurrentContext(), &win_width, &win_height);
        glViewport(0, 0, win_width, win_height);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::SetNextWindowSize({ float(win_width), float(win_height) });
        ImGui::SetNextWindowPos({ 0, 0 });
        if (ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration))
        {
            mk_button<default_impl>(state, "Default Impl");
            mk_button<automaton_impl>(state, "Automaton Impl");
            mk_button<caves_impl>(state, "Caves Impl");

            ImGui::End();
        }
    }
    void menu_impl::on_end(program_state& state)
    {
    }
}