#include <processing/gl_clock.hpp>
#include <opengl/mygl.hpp>

namespace mpp
{
    gl_query_clock::time_point gl_query_clock::now() noexcept
    {
        std::uint32_t time_stamp_query;
        glGenQueries(1, &time_stamp_query);

        glQueryCounter(time_stamp_query, GL_TIMESTAMP);
        std::uint64_t time_stamp = 0;
        glGetQueryObjectui64v(time_stamp_query, GL_QUERY_RESULT, &time_stamp);

        glDeleteQueries(1, &time_stamp_query);
        return time_point{ duration(time_stamp) };
    }
}