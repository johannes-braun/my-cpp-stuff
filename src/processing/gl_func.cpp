#include <processing/gl_func.hpp>
#include <opengl/mygl.hpp>
#include <spdlog/spdlog.h>

namespace mpp
{
    std::uint32_t create_shader(GLenum type, const char* src)
    {
        std::uint32_t sh = glCreateShader(type);
        glShaderSource(sh, 1, &src, nullptr);
        glCompileShader(sh);
        int log_len;
        glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 3)
        {
            std::string info_log(log_len, '\0');
            glGetShaderInfoLog(sh, log_len, &log_len, info_log.data());
            spdlog::info("Shader Compilation Output:\n{}", info_log);
        }
        return sh;
    }

    std::uint32_t create_program(std::initializer_list<std::uint32_t> shaders)
    {
        std::uint32_t program = glCreateProgram();
        for (auto s : shaders)
            glAttachShader(program, s);
        glLinkProgram(program);
        int log_len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
        if (log_len > 3)
        {
            std::string info_log(log_len, '\0');
            glGetProgramInfoLog(program, log_len, &log_len, info_log.data());
            spdlog::info("Shader Compilation Output:\n{}", info_log);
        }
        for (auto s : shaders)
            glDetachShader(program, s);
        return program;
    }
}