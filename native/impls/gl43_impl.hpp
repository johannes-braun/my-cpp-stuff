#pragma once

#include <visualization.hpp>
#include <cstdint>
#include <processing/sift/sift.hpp>
#include <processing/image.hpp>

namespace mpp
{
    class gl43_impl : public basic_visualization
    {
    public:
        gl43_impl();
        void on_setup(program_state& state) final override;
        void on_start(program_state& state) final override;
        void on_update(program_state& state, seconds delta) final override;
        void on_end(program_state& state) final override;

    private:
        void add_img(const char* path);
        struct
        {
            std::uint32_t program;
            std::int32_t  in_texture_location;
        } full_screen;

        struct
        {
            std::uint32_t program;
            std::uint32_t vao;
            std::uint32_t vbo;
            std::uint32_t ori_vbo;
            std::int32_t  u_color_location;
        } points;

        std::shared_ptr<sift::sift_cache> _sift_cache;
        std::uint32_t empty_vao;
        image img;
        std::vector<std::uint32_t> textures;
        std::vector<std::vector<sift::feature>> features;
        std::vector<std::pair<glm::vec2, glm::vec2>> ref;
        std::vector<std::vector<glm::vec2>> orientation_dbg;
        float point_size = 4.f;
        int current_texture = 0;
        int num_matches = -1;
    };
}