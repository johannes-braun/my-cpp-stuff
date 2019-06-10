#pragma once

#include <platform/environment.hpp>

struct GLFWwindow;
namespace mpp
{
    class opengl_environment : public detail::environment_interface
    {
    public:
        void on_setup(program_state& state) final override;
        void on_start(program_state& state) final override;
        void on_begin_update(program_state& state, seconds delta) final override;
        void on_end_update(program_state& state, seconds delta) final override;
        void on_end(program_state& state) final override;

    };
}