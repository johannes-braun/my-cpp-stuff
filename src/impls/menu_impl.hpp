#pragma once

#include <impl.hpp>

namespace mpp
{
    class menu_impl : public impl
    {
    public:
        ~menu_impl();
        void on_start(program_state& state) final override;
        void on_update(program_state& state, std::chrono::steady_clock::time_point::duration delta) final override;
        void on_end(program_state& state) final override;
        
    private:

    };
}