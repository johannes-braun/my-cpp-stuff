#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <chrono>
#include <optional>

/*
const auto delta_millis = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(delta);
const auto gcc = glfwGetCurrentContext();
double cposx, cposy;
glfwGetCursorPos(gcc, &cposx, &cposy);

if (!ImGui::GetIO().WantCaptureKeyboard && !ImGui::GetIO().WantTextInput)
{
    _camera.input_axis_x(glfwGetKey(gcc, GLFW_KEY_D) - glfwGetKey(gcc, GLFW_KEY_A));
    _camera.input_axis_y(glfwGetKey(gcc, GLFW_KEY_E) - glfwGetKey(gcc, GLFW_KEY_Q));
    _camera.input_axis_z(glfwGetKey(gcc, GLFW_KEY_W) - glfwGetKey(gcc, GLFW_KEY_S));
}

if (!ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(gcc, GLFW_MOUSE_BUTTON_LEFT))
{
    _camera.input_rotate_h(cposx - _last_curpos.x);
    _camera.input_rotate_v(cposy - _last_curpos.y);

    glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
else {
    glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
_last_curpos = { cposx, cposy };
_camera.update(delta_millis);
*/

namespace mpp
{
    class camera
    {
    public:
        camera() = default;

        glm::mat4 model_matrix() const;
        glm::mat4 view_matrix() const;

        void update(std::chrono::duration<double, std::milli> delta);

        void set_position(glm::vec3 position, bool smooth = false);
        void look_at(glm::vec3 dst, std::optional<glm::vec3> up = std::nullopt);
        void look_at(glm::vec3 dst, bool smooth, std::optional<glm::vec3> up = std::nullopt);
        void set_axis_smoothing(float smoothing);
        void set_rotate_smoothing(float smoothing);
        void set_axis_scaling(std::optional<float> scaling);
        void set_rotate_scaling(std::optional<float> scaling);
        
        glm::vec3 dir_front() const;
        glm::vec3 dir_back() const;
        glm::vec3 dir_right() const;
        glm::vec3 dir_left() const;
        glm::vec3 dir_up() const;
        glm::vec3 dir_down() const;

        void input_speed_factor(float factor);
        void input_axis_x(float val);
        void input_axis_y(float val);
        void input_axis_z(float val);
        void input_rotate_v(float rot);
        void input_rotate_h(float rot);
        
    private:
        glm::vec3 _input_axis{ 0, 0, 0 };
        glm::vec2 _input_rotate{ 0, 0 };
        float _input_speed_factor{ 0.f };
        float _axis_smoothing{ 0.5f };
        float _rotate_smoothing{ 0.5f };

        std::optional<float> _axis_scaling = std::nullopt; // use delta time.
        std::optional<float> _rotate_scaling = 0.003f;     // use fixed scaling.

        glm::quat _rotation{ 1, 0, 0, 0 };
        glm::vec3 _position{ 0, 0, 0 };

        // Smooth control
        glm::quat _dst_rotation{ 1, 0, 0, 0 };
        glm::vec3 _dst_position{ 0, 0, 0 };
    };
}