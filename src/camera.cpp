#include <camera.hpp>

namespace mpp
{
    glm::mat4 camera::model_matrix() const
    {
        return glm::translate(glm::mat4(1.f), _position)* glm::mat4_cast(_rotation);
    }
    glm::mat4 camera::view_matrix() const
    {
        return inverse(model_matrix());
    }

    void camera::set_position(glm::vec3 position, bool smooth)
    {
        _dst_position = position;
        if (!smooth) _position = _dst_position;
    }

    void camera::look_at(glm::vec3 dst, std::optional<glm::vec3> up)
    {
        look_at(dst, false, up);
    }

    void camera::look_at(glm::vec3 dst, bool smooth, std::optional<glm::vec3> up)
    {
        assert(dst != _dst_position);
        _dst_rotation = glm::quatLookAt(glm::normalize(dst - _dst_position), up ? *up : dir_up());
        if (!smooth)
            _rotation = _dst_rotation;
    }

    void camera::set_axis_smoothing(float smoothing)
    {
        _axis_smoothing = smoothing;
    }

    void camera::set_rotate_smoothing(float smoothing)
    {
        _rotate_smoothing = smoothing;
    }

    void camera::set_axis_scaling(std::optional<float> scaling)
    {
        _axis_scaling = scaling;
    }

    void camera::set_rotate_scaling(std::optional<float> scaling)
    {
        _rotate_scaling = scaling;
    }

    void camera::update(std::chrono::duration<double, std::milli> delta)
    {
        const float delta_seconds = float(delta.count()) / 1000.f;
        const float pos_scale = _axis_scaling ? *_axis_scaling : delta_seconds;
        _dst_position += delta_seconds * _input_axis.x * dir_right();
        _dst_position += delta_seconds * _input_axis.y * dir_up();
        _dst_position += delta_seconds * _input_axis.z * dir_front();

        const float rot_scale = _rotate_scaling ? *_rotate_scaling : delta_seconds;
        _dst_rotation = glm::angleAxis(rot_scale * _input_rotate.x, glm::vec3(0, 1, 0)) * _dst_rotation * glm::angleAxis(rot_scale * _input_rotate.y, glm::vec3(1, 0, 0));

        _position = glm::mix(_position, _dst_position, glm::pow(delta_seconds, _axis_smoothing));
        _rotation = glm::slerp(_rotation, _dst_rotation, glm::pow(delta_seconds, _rotate_smoothing));

        _input_rotate = { 0, 0 };
        _input_axis = { 0, 0, 0 };
    }
    glm::vec3 camera::dir_front() const
    {
        return _rotation * glm::vec3(0, 0, -1);
    }
    glm::vec3 camera::dir_back() const
    {
        return _rotation * glm::vec3(0, 0, 1);
    }
    glm::vec3 camera::dir_right() const
    {
        return _rotation * glm::vec3(1, 0, 0);
    }
    glm::vec3 camera::dir_left() const
    {
        return _rotation * glm::vec3(-1, 0, 0);
    }
    glm::vec3 camera::dir_up() const
    {
        return _rotation * glm::vec3(0, 1, 0);
    }
    glm::vec3 camera::dir_down() const
    {
        return _rotation * glm::vec3(0, -1, 0);
    }
    void camera::input_speed_factor(float factor)
    {
        _input_speed_factor = factor;
    }
    void camera::input_axis_x(float val)
    {
        _input_axis.x += _input_speed_factor * val;
    }
    void camera::input_axis_y(float val)
    {
        _input_axis.y += _input_speed_factor * val;
    }
    void camera::input_axis_z(float val)
    {
        _input_axis.z += _input_speed_factor * val;
    }
    void camera::input_rotate_v(float rot)
    {
        _input_rotate.y -= rot;

    }
    void camera::input_rotate_h(float rot)
    {
        _input_rotate.x -= rot;
    }
}