#pragma once

#include <cstdint>
#include <initializer_list>
#include <opengl/mygl_enums.hpp>

namespace mpp
{
    std::uint32_t create_shader(GLenum type, const char* src);
    std::uint32_t create_program(std::initializer_list<std::uint32_t> shaders);
}