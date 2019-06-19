#pragma once
#include <chrono>
#include <string>
#include <spdlog/spdlog.h>

namespace mpp
{
    namespace detail
    {
        template<typename Duration>
        constexpr const char* duration_type_name();
        template<>
        constexpr const char* duration_type_name<std::chrono::nanoseconds>()
        {
            return "ns";
        }
        template<>
        constexpr const char* duration_type_name<std::chrono::microseconds>()
        {
            return "us";
        }
        template<>
        constexpr const char* duration_type_name<std::chrono::milliseconds>()
        {
            return "ms";
        }
        template<>
        constexpr const char* duration_type_name<std::chrono::seconds>()
        {
            return "s";
        }
        template<>
        constexpr const char* duration_type_name<std::chrono::minutes>()
        {
            return "min";
        }
        template<>
        constexpr const char* duration_type_name<std::chrono::hours>()
        {
            return "h";
        }
    }

    template<typename Duration, typename Clock>
    class basic_perf_log
    {
    public:
        using clock = Clock;
        using time_point = typename clock::time_point;
        using duration = Duration;
        inline static constexpr const char* duration_name = detail::duration_type_name<duration>();

        basic_perf_log(std::string name) : _name(std::move(name)) {}
        ~basic_perf_log() { if (_running) finish(); }
        void start() {
            if (!_running)
            {
                spdlog::info("Starting PerfLog [{}]", _name);
                _time_begin = _time_last_step = clock::now();
                _running = true;
            }
            else
            {
                spdlog::warn("Trying to start PerfLog [{}], but it is already running", _name);
            }
        }
        void step(const std::string& description)
        {
            if (_running)
            {
                const auto dur = std::chrono::duration_cast<duration>(clock::now() - _time_last_step);
                spdlog::info("[{} : {} {}] {}", _name, dur.count(), duration_name, description);
                _time_last_step = clock::now();
            }
            else
            {
                spdlog::warn("Trying to step PerfLog [{}], but it is not running", _name);
            }
        }
        void finish()
        {
            if (_running)
            {
                const auto dur = std::chrono::duration_cast<duration>(clock::now() - _time_begin);
                spdlog::info("Finishing PerfLog [{}] after {} {}", _name, dur.count(), duration_name);
                _running = false;
            }
            else
            {
                spdlog::warn("Trying to finish PerfLog [{}], but it is not running", _name);
            }
        }

    private:
        std::string _name;
        bool _running = false;
        time_point _time_begin;
        time_point _time_last_step;
    };

    using perf_log = basic_perf_log<std::chrono::milliseconds, std::chrono::system_clock>;
}