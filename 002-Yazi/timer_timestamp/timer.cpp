#include <timer/timer.h>

using namespace demo::timer;

Timer::Timer() : m_period(0), m_repeat(-1)
{
    m_time = now();
}

Timer::Timer(int repeat) : m_period(0), m_repeat(repeat)
{
    m_time = now();
}

int64_t Timer::now()
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    return now_ms.time_since_epoch().count();
}

void Timer::on_timer()
{
    if (!m_func || 0 == m_repeat)
        return;

    m_func();
    m_time += m_period;

    if (0 < m_repeat)
        --m_repeat;
}