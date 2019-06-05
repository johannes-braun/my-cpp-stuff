#pragma once

#include <visualization.hpp>

namespace mpp
{
    class gl43_impl : public basic_visualization
    {
    public:
        void on_setup(program_state& state) final override;
        void on_start(program_state& state) final override;
        void on_update(program_state& state, seconds delta) final override;
        void on_end(program_state& state) final override;

    private:

    };
}