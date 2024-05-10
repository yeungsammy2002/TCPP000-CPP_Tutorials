#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <chrono>

namespace demo {
namespace utilities {

class Timer
{
public:
    Timer();

    Timer(int repeat);

    ~Timer();

    template<typename F, typename... Args>
    void start(int milliseconds, F && f, Args && ...args);

    void stop();

private:
    std::thread m_thread;
    std::atomic<bool> m_active;
    std::function<void()> m_func;
    int m_period;
    int m_repeat;
};

#include <utilities/timer.inl>

}
}