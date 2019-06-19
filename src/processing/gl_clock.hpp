#pragma once

#include <chrono>

namespace mpp
{
    struct gl_query_clock
    {
        using duration = std::chrono::nanoseconds;
        using rep = duration::rep;
        using period = duration::period;
        using time_point = std::chrono::time_point<gl_query_clock>;
        inline static constexpr bool is_steady = false;

        static time_point now() noexcept;
    };
}