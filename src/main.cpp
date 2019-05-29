#include <program_state.hpp>
#include <impls/default_impl.hpp>

int main(int argc, char** argv)
{
    mpp::program_state state;
    state.open_state(std::make_shared<mpp::default_impl>());
    state.poll();
}
