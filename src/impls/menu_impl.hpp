#pragma once

#include <visualization.hpp>

namespace mpp
{
    class menu_impl : public basic_visualization
    {
    public:
        ~menu_impl();
        void on_start(program_state& state) final override;
        void on_update(program_state& state, seconds delta) final override;
        void on_end(program_state& state) final override;
        
    private:

    };
}