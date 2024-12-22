#pragma once

#include <stdint.h>
#include <functional>
#include <chrono>

class Timer
{
public:
    Timer();
    Timer(int repeat);
    ~Timer();

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

#include <utilities/timer2.inl>
