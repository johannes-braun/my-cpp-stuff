#pragma once

#include <imgui/imgui.h>
#include <impl.hpp>
#include <glm/glm.hpp>
#include <camera.hpp>
#include <vector>

namespace mpp
{
    class automaton_impl : public impl
    {
    public:
        ~automaton_impl();
        void on_setup(program_state& state) final override;
        void on_start(program_state& state) final override;
        void on_update(program_state& state, std::chrono::steady_clock::time_point::duration delta) final override;
        void on_end(program_state& state) final override;

    private:
        struct cell_t
        {
            int value;
        };

        glm::ivec2 _texture_size;
        std::uint32_t _texture;
        std::uint32_t _fbo;
        std::vector<cell_t> _cells_front;
        std::vector<cell_t> _cells_back;
        std::vector<glm::vec4> _tex_data;
        bool _running = false;
        int _radius = 3;
        std::chrono::steady_clock::time_point::duration _acc_time;
    };
}