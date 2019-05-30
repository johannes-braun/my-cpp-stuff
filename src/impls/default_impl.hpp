#pragma once

#include <imgui/imgui.h>
#include <impl.hpp>
#include <glm/glm.hpp>
#include <camera.hpp>

namespace mpp
{
    class default_impl : public impl
    {
    public:
        ~default_impl();
        void on_setup(program_state& state) final override;
        void on_start(program_state& state) final override;
        void on_update(program_state& state, std::chrono::steady_clock::time_point::duration delta) final override;
        void on_end(program_state& state) final override;

    private:
        std::uint32_t _vbo;
        std::uint32_t _vao;
        std::uint32_t _program;
        glm::vec3 _clear_color;

        glm::dvec2 _last_curpos;
        camera _camera;
    };
}