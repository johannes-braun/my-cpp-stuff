#pragma once

#include <chrono>

namespace mpp
{
    class program_state;
    class impl
    {
    public:
        virtual ~impl() = default;
        virtual void on_create(program_state& state) = 0;
        virtual void on_update(program_state& state, std::chrono::steady_clock::time_point::duration delta) = 0;
        virtual void on_destroy(program_state& state) = 0;
    };
}