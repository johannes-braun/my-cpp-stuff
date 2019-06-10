
#include <impls/menu_impl.hpp>
#include <imgui/imgui.h>
#include <program_state.hpp>
#include <opengl/mygl_glfw.hpp>
#include <fstream>

#include <impls/automaton_impl.hpp>
#include <impls/default_impl.hpp>
#include <impls/caves_impl.hpp>
#include <impls/gl43_impl.hpp>
#include <processing/image.hpp>
#include <platform/opengl.hpp>

namespace mpp
{
    menu_impl::menu_impl()
    {
        use_environment<opengl_environment>();
    }
    menu_impl::~menu_impl()
    {
    }
    void menu_impl::on_start(program_state& state)
    {
        glfwSetWindowSize(glfwGetCurrentContext(), 300, 400);

        glGenTextures(1, &_logo);
        glBindTexture(GL_TEXTURE_2D, _logo);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);

        image i;
        i.load_stream(std::ifstream("../../res/logo.png", std::ios::binary), 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, i.dimensions().x, i.dimensions().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, i.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    namespace {
        template<typename Impl>
        void mk_button(program_state& state, const char* text)
        {
            if (ImGui::Button(text, ImVec2(ImGui::GetWindowContentRegionWidth(), 0)))
                state.open_state(std::make_shared<Impl>());
        }
    }

    void menu_impl::on_update(program_state& state, seconds delta)
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
            ImGui::Image(reinterpret_cast<ImTextureID>(std::uint64_t(_logo)), ImVec2(64, 64));
            ImGui::SameLine();
            if (ImGui::BeginChild("##Child1", ImVec2(0, 64)))
            {
                ImGui::Dummy(ImVec2(0.0f, 8.0f));
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
                ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.f, 1.f), "MPP");
                ImGui::PopFont();
                ImGui::TextColored(ImVec4(1, 1, 1, 1.f), "My C++ Stuff");
                ImGui::EndChild();
            }
            mk_button<default_impl>(state, "Default Impl");
            mk_button<automaton_impl>(state, "Automaton Impl");
            mk_button<caves_impl>(state, "Caves Impl");
            mk_button<gl43_impl>(state, "OpenGL 4.3 Impl");

            ImGui::End();
        }
    }
    void menu_impl::on_end(program_state& state)
    {
        glDeleteTextures(1, &_logo);
    }
}