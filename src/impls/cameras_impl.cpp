#include "cameras_impl.hpp"
#include <opengl/mygl_glfw.hpp>
#include <program_state.hpp>
#include <iostream>
#include <glm/ext.hpp>
#include <platform/opengl.hpp>
#include <tinyfd/fd.hpp>
#include <spdlog/spdlog.h>
#include <fstream>

namespace mpp
{
    cameras_impl::cameras_impl() {
        use_environment<opengl_environment>();
    }
    void cameras_impl::on_setup(program_state& state)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_SAMPLES, 4);
    }
    void cameras_impl::on_start(program_state& state) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        _photogrammetry.run();
        _camera.set_axis_smoothing(0.7f);
        _camera.set_rotate_smoothing(0.7f);
        _camera.set_position(glm::vec3(3, 3, 3));
        _camera.look_at(glm::vec3(0, 0, 0));
    }
    void cameras_impl::on_update(program_state & state, seconds delta) {

        const auto delta_millis = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(delta);
        const auto gcc = glfwGetCurrentContext();
        double cposx, cposy;
        glfwGetCursorPos(gcc, &cposx, &cposy);

        if (!ImGui::GetIO().WantCaptureKeyboard && !ImGui::GetIO().WantTextInput)
        {
            _camera.input_axis_x(float(glfwGetKey(gcc, GLFW_KEY_D) - glfwGetKey(gcc, GLFW_KEY_A)));
            _camera.input_axis_y(float(glfwGetKey(gcc, GLFW_KEY_E) - glfwGetKey(gcc, GLFW_KEY_Q)));
            _camera.input_axis_z(float(glfwGetKey(gcc, GLFW_KEY_W) - glfwGetKey(gcc, GLFW_KEY_S)));
            if (glfwGetKey(gcc, GLFW_KEY_LEFT_CONTROL))
                _camera.input_speed_factor(0.6f);
            else if(glfwGetKey(gcc, GLFW_KEY_LEFT_SHIFT))
                _camera.input_speed_factor(9.f);
            else
                _camera.input_speed_factor(3.f);
        }

        if (!ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(gcc, GLFW_MOUSE_BUTTON_LEFT))
        {
            _camera.input_rotate_h(float(cposx - _last_curpos.x));
            _camera.input_rotate_v(float(cposy - _last_curpos.y));

            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        _last_curpos = { cposx, cposy };
        _camera.update(delta_millis);
        glClear(GL_COLOR_BUFFER_BIT);

        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        const glm::mat4 view_proj =
            glm::perspectiveFov(glm::radians(60.f), float(viewport[2]), float(viewport[3]), 0.01f, 100.f) *
            _camera.view_matrix();

        if (ImGui::Begin("Settings"))
        {
            ImGui::Text("Sizes");
            ImGui::DragInt("Scales", reinterpret_cast<int*>(&_photogrammetry.base_processor().detection_settings().feature_scales), 0.01f, 0, 10);
            ImGui::DragInt("Octaves", reinterpret_cast<int*>(&_photogrammetry.base_processor().detection_settings().octaves), 0.01f, 0, 10);
            ImGui::Text("Orientation");
            ImGui::DragFloat("Magnitude Thresh.", &_photogrammetry.base_processor().detection_settings().orientation_magnitude_threshold, 0.000002f, 0.f, 1.f);
            ImGui::DragInt("Slices", &_photogrammetry.base_processor().detection_settings().orientation_slices, 0.01f, 0, 360);
            if (ImGui::Button("Open Images", ImVec2(ImGui::GetContentRegionAvailWidth(), 0)))
            {
                auto ps = open_files("Open Images");
                _img_counter = 0;
                _imgs_to_load = ps.size();
                for (const auto& path : ps)
                {
                    emplace_file(path);
                }
                _photogrammetry.detect_all();
            }
            if(_imgs_to_load != 0 && _imgs_to_load != _img_counter.load())
                ImGui::ProgressBar(float(_img_counter.load()) / _imgs_to_load);

            ImGui::Spacing();
            ImGui::DragInt("Max. Matches", &_photogrammetry.base_processor().match_settings().max_match_count, 0.01f, 0, 2500);
            ImGui::DragFloat("Relation Thresh.", &_photogrammetry.base_processor().match_settings().relation_threshold, 0.001f, 0.f, 1.f);
            ImGui::DragFloat("Similarity Thresh.", &_photogrammetry.base_processor().match_settings().similarity_threshold, 0.001f, 0.f, 1.f);
            
            if (ImGui::Button("Match", ImVec2(ImGui::GetContentRegionAvailWidth(), 0)))
            {
                _photogrammetry.match_all();
            }
        }
        ImGui::End();
    }
    void cameras_impl::on_end(program_state & state) {

    }
    void cameras_impl::emplace_file(const std::filesystem::path& path)
    {
        spdlog::info("Adding file \"{}\".", path.string());
        std::shared_ptr<image> img = std::make_shared<image>();
        std::ifstream file(path, std::ios::binary | std::ios::in);
        img->load_stream(file, 1);
        _photogrammetry.add_image(std::move(img), 0.028f, [this] {_img_counter++; });
    }
}