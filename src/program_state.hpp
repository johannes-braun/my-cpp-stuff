#pragma once

#include <impl.hpp>
#include <mutex>
#include <thread>
#include <memory>
#include <atomic>
#include <unordered_map>
#include <vector>

struct GLFWwindow;
struct ImGuiContext;
namespace mpp
{
    class program_state
    {
    public:
        void poll();
        void open_state(std::shared_ptr<mpp::impl> state);

    private:
        std::mutex _state_mutex;
        std::unordered_map<std::shared_ptr<mpp::impl>, std::thread> _state_threads;
        std::vector<std::shared_ptr<mpp::impl>> _to_remove;
        std::atomic_int _num_windows = 0;
    };
}