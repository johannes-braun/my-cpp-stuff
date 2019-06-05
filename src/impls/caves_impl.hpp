#pragma once

#include <imgui/imgui.h>
#include <visualization.hpp>
#include <glm/glm.hpp>
#include <camera.hpp>
#include <vector>
#include <random>

namespace mpp
{
    class caves_impl : public basic_visualization
    {
    public:
        ~caves_impl();
        void on_setup(program_state& state) final override;
        void on_start(program_state& state) final override;
        void on_update(program_state& state, seconds delta) final override;
        void on_end(program_state& state) final override;

    private:
        struct cell_t
        {
            int value;
        };

        // Marching cubes
        std::uint32_t _mc_buf_case_faces;
        std::uint32_t _mc_buf_edge_connections;
        std::uint32_t _mc_program;
        std::uint32_t _mc_vao;
        void init_mc();

        // Rest
        glm::ivec3 _texture_size;
        std::uint32_t _texture_front;
        std::uint32_t _texture_back;
        std::uint32_t _sim_program;
        std::uint32_t _mesh_program;
        std::uint32_t _vao;
        std::uint32_t _vbo;
        std::uint32_t _draw_program;
        float _min_cutoff = 0.5f;
        float _max_cutoff = 0.5f;

        glm::dvec2 _last_curpos;
        camera _camera;

        float _base_point_size = 2.f;
        bool _running = false;
        int _radius = 3;
        seconds _acc_time;

        std::mt19937 _rng;
        std::uniform_int_distribution<int> _dist{ 0, 1 };
    };
}