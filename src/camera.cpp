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
    void camera::set_position(glm::vec3 position)
    {
        _position = _dst_position = position;
    }

    void camera::look_at(glm::vec3 dst, std::optional<glm::vec3> up)
    {
        _rotation = _dst_rotation = glm::quatLookAt(glm::normalize(dst - _position), up ? *up : dir_up());
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

        _position = glm::mix(_position, _dst_position, glm::pow(delta_seconds, 0.6f));
        _rotation = glm::slerp(_rotation, _dst_rotation, glm::pow(delta_seconds, 0.6f));

        _input_rotate = {0, 0};
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
    void camera::input_axis_x(float val)
    {
        _input_axis.x += val;
    }
    void camera::input_axis_y(float val)
    {
        _input_axis.y += val;
    }
    void camera::input_axis_z(float val)
    {
        _input_axis.z += val;
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