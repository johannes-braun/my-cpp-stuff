#pragma once

#include <imgui/imgui.h>
#include <visualization.hpp>
#include <glm/glm.hpp>
#include <camera.hpp>
#include <processing/photogrammetry.hpp>
#include <filesystem>

namespace mpp
{
    class cameras_impl : public basic_visualization
    {
    public:
        cameras_impl();
        void on_setup(program_state& state) final override;
        void on_start(program_state& state) final override;
        void on_update(program_state& state, seconds delta) final override;
        void on_end(program_state& state) final override;

    private:
        void emplace_file(const std::filesystem::path& img);

        photogrammetry_processor _photogrammetry;
        glm::dvec2 _last_curpos;
        camera _camera;
    };
}