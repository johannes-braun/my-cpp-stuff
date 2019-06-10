#include <visualization.hpp>
#include <iostream>

namespace mpp
{
    void visualization_interface::setup(program_state& state)
    {
        if (_active_environments.empty())
            std::cerr << "No active environment.\n";
        for (auto& env : _active_environments)
        {
            env->on_setup(state);
        }
        on_setup(state);
    }
    void visualization_interface::start(program_state& state)
    {
        for (auto& env : _active_environments)
        {
            env->on_start(state);
        }
        on_start(state);
    }
    void visualization_interface::begin_update(program_state& state, seconds delta)
    {
        for (auto& env : _active_environments)
        {
            env->on_begin_update(state, delta);
        }
    }
    void visualization_interface::update(program_state& state, seconds delta)
    {
        on_update(state, delta);
    }
    void visualization_interface::end_update(program_state& state, seconds delta)
    {
        for (auto& env : _active_environments)
        {
            env->on_end_update(state, delta);
        }
    }
    void visualization_interface::end(program_state& state)
    {
        for (auto& env : _active_environments)
        {
            env->on_end(state);
        }
        on_end(state);
    }
}