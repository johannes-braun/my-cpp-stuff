#pragma once

#include <chrono>
#include <vector>
#include <memory>
#include <platform/environment.hpp>

namespace mpp
{
    class program_state;

    class visualization_interface
    {
    public:
        friend program_state;
        using seconds = std::chrono::duration<double>;
        using milliseconds = std::chrono::duration<double, std::milli>;

        virtual ~visualization_interface() = default;
        virtual void on_setup(program_state& state) = 0;
        virtual void on_start(program_state& state) = 0;
        virtual void on_update(program_state& state, seconds delta) = 0;
        virtual void on_end(program_state& state) = 0;

    protected:
        template<typename T, typename ... CtorArgs>
        void use_environment(CtorArgs&& ... args);

    private:
        void setup(program_state& state);
        void start(program_state& state);
        void begin_update(program_state& state, seconds delta);
        void update(program_state& state, seconds delta);
        void end_update(program_state& state, seconds delta);
        void end(program_state& state);
        std::vector<std::unique_ptr<detail::environment_interface>> _active_environments;
    };

    class basic_visualization : public visualization_interface
    {
    public:
        virtual void on_setup(program_state& state) {};
        virtual void on_start(program_state& state) {};
        virtual void on_end(program_state& state) {};
    };

    template<typename T, typename ...CtorArgs>
    void visualization_interface::use_environment(CtorArgs&& ...args)
    {
        _active_environments.emplace_back(std::make_unique<T>(std::forward<CtorArgs>(args)...));
    }
}