#pragma once

#include <chrono>

namespace mpp
{
    class program_state;
}

namespace mpp
{
    namespace detail
    {
        class environment_interface
        {
        public:
            using seconds = std::chrono::duration<double>;

            virtual ~environment_interface() = default;
            virtual void on_setup(program_state& state) = 0;
            virtual void on_start(program_state& state) = 0;
            virtual void on_begin_update(program_state& state, seconds delta) = 0;
            virtual void on_end_update(program_state& state, seconds delta) = 0;
            virtual void on_end(program_state& state) = 0;
        };
    }

    class basic_environment : public detail::environment_interface
    {
    public:
        void on_setup(program_state& state) override {}
        void on_start(program_state& state) override {}
        void on_begin_update(program_state& state, seconds delta) override {}
        void on_end_update(program_state& state, seconds delta) override {}
        void on_end(program_state& state) override {}
    };
}