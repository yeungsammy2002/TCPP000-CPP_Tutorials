#pragma once

#include <cstdint>
#include <functional>
#include <chrono>

namespace demo {
namespace timer {

class Timer
{
    friend class TimerManager;

public:
    Timer();

    Timer(int repeat);

    ~Timer() = default;

    template<typename F, typename... Args>
    void callback(int milliseconds, F && f, Args && ... args);

    void on_timer();

private:
    static int64_t now();

private:
    int64_t m_time;
    std::function<void()> m_func;
    int m_period;
    int m_repeat;
};

template<typename F, typename... Args>
void Timer::callback(int milliseconds, F && f, Args && ... args)
{
    m_period = milliseconds;
    m_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
}

}
}