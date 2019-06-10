#include "automaton_impl.hpp"
#include <opengl/mygl_glfw.hpp>
#include <program_state.hpp>
#include <iostream>
#include <glm/ext.hpp>
#include <platform/environment.hpp>
#include <platform/opengl.hpp>

namespace mpp
{
    automaton_impl::automaton_impl()
    {
        use_environment<opengl_environment>();
    }

    automaton_impl::~automaton_impl() {}
    void automaton_impl::on_setup(program_state& state)
    {
    }
    void automaton_impl::on_start(program_state& state) {
        _texture_size = { 200, 100 };

        glCreateTextures(GL_TEXTURE_2D, 1, &_texture);
        glTextureStorage2D(_texture, 1, GL_RGBA8, _texture_size.x, _texture_size.y);

        _cells_back.resize(_texture_size.x * _texture_size.y);
        _tex_data.resize(_texture_size.x * _texture_size.y);
        for (int y = 0; y < _texture_size.y; ++y)
        {
            for (int x = 0; x < _texture_size.x; ++x)
            {
                _cells_back[y * _texture_size.x + x].value = rand() % 2;// glm::simplex(glm::vec2(x, y) / 50.f) > 0.4f ? 1 : 0;
            }
        }
        _cells_front = _cells_back;

        glCreateFramebuffers(1, &_fbo);
        glNamedFramebufferTexture(_fbo, GL_COLOR_ATTACHMENT0, _texture, 0);
        glTextureSubImage2D(_texture, 0, 0, 0, _texture_size.x, _texture_size.y, GL_RGBA, GL_FLOAT, _tex_data.data());
    }
    void automaton_impl::on_update(program_state& state, seconds delta) {
        _acc_time += delta;
        int w, h;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &w, &h);
        double cx, cy;
        glfwGetCursorPos(glfwGetCurrentContext(), &cx, &cy);
        const auto tc_idx = [&](auto x, auto y)
        {
            x = glm::clamp(x, decltype(x)(0), decltype(x)(_texture_size.x - 1));
            y = glm::clamp(y, decltype(y)(0), decltype(y)(_texture_size.y - 1));
            return y * _texture_size.x + x;
        };

        if (!ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT))
        {
            const auto px = _texture_size.x * cx / w;
            const auto py = _texture_size.y - 1 - _texture_size.y * cy / h;

            for(int i = -_radius; i <= _radius; ++i)
                for(int j = -_radius; j <= _radius; ++j)
                _cells_front[tc_idx(int(px)+i, int(py)+j)].value = 1;
        }
        if (!ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT))
        {
            const auto px = _texture_size.x * cx / w;
            const auto py = _texture_size.y - _texture_size.y * cy / h;

            for (int i = -_radius; i <= _radius; ++i)
                for (int j = -_radius; j <= _radius; ++j)
                    _cells_front[tc_idx(int(px) + i, int(py) + j)].value = 0;
        }

        if (_running && _acc_time > std::chrono::milliseconds(30))
        {
            // update
            for (int y = 0; y < _texture_size.y; ++y)
            {
                for (int x = 0; x < _texture_size.x; ++x)
                {
                    const auto c = _cells_front[tc_idx(x, y)];
                    cell_t neighs[8];
                    int neigh = 0;
                    for (auto nx = -1; nx <= 1; ++nx)
                    {
                        for (auto ny = -1; ny <= 1; ++ny)
                        {
                            if (nx != 0 || ny != 0)
                            {
                                neighs[neigh++] = _cells_front[tc_idx(x + nx, y + ny)];
                            }
                        }
                    }

                    float nval = 0.f;
                    float dif = 0.f;
                    for (int i = 0; i < 8; ++i)
                    {
                        if (neighs[i].value)
                            nval += 1.f;
                        dif += (neighs[i].value - c.value);
                    }

                    if (!_cells_back[tc_idx(x, y)].value && nval > 4.f)
                        _cells_back[tc_idx(x, y)].value = 1;
                    else if (_cells_back[tc_idx(x, y)].value && nval < 4.f)
                        _cells_back[tc_idx(x, y)].value = 0;

                    //_cells_back[tc_idx(x, y)].value += dif / 8.f;

                    //if(dif > 1.f)
                    //    _cells_back[tc_idx(x, y)].value = 0;
                    //else
                    //    _cells_back[tc_idx(x, y)].value = 1;


                 /*   if ((nval == 2 && c.value == 1) || nval == 3)
                        _cells_back[tc_idx(x, y)].value = 1;
                    else
                        _cells_back[tc_idx(x, y)].value = 0;*/
                }
            }
            _cells_front = _cells_back;
            _acc_time = { 0 };
        }

        int i = 0;
        for (const auto& x : _cells_front)
        {
            _tex_data[i++] = glm::vec4(glm::vec3(x.value ? 1.f : 0.f), 1.f);
        }
        glTextureSubImage2D(_texture, 0, 0, 0, _texture_size.x, _texture_size.y, GL_RGBA, GL_FLOAT, _tex_data.data());

        if (ImGui::Begin("Settings"))
        {
            ImGui::Checkbox("Run", &_running);
            ImGui::DragInt("Radius", &_radius, 0.1f, 0, 1000);
            if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionAvailWidth(), 0)))
            {
                for (int y = 0; y < _texture_size.y; ++y)
                {
                    for (int x = 0; x < _texture_size.x; ++x)
                    {
                        _cells_back[y * _texture_size.x + x].value = rand() % 2;// glm::simplex(glm::vec2(x, y) / 50.f) > 0.4f ? 1 : 0;
                    }
                }
                _cells_front = _cells_back;
            }
            ImGui::End();
        }

        glBlitNamedFramebuffer(_fbo, 0, 0, 0, _texture_size.x, _texture_size.y, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    void automaton_impl::on_end(program_state & state) {

    }
}