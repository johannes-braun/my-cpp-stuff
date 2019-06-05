#pragma once

#include <chrono>

namespace mpp
{
    class program_state;
    class visualization_interface
    {
    public:
        using seconds = std::chrono::duration<double>;
        using milliseconds = std::chrono::duration<double, std::milli>;

        virtual ~visualization_interface() = default;
        virtual void on_setup(program_state& state) = 0;
        virtual void on_start(program_state& state) = 0;
        virtual void on_update(program_state& state, seconds delta) = 0;
        virtual void on_end(program_state& state) = 0;
    };

    class basic_visualization : public visualization_interface
    {
    public:
        virtual void on_setup(program_state& state) {};
        virtual void on_start(program_state& state) {};
        virtual void on_end(program_state& state) {};
    };
}