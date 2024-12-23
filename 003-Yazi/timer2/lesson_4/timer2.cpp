#include <utilities/timer2.h>

Timer2::Timer2() : m_period(0), m_repeat(-1)
{
    m_time = now();

}

Timer2::Timer2(int repeat) : m_period(0), m_repeat(repeat)
{
    m_time = now();
}

Timer2::~Timer2()
{

}

int64_t Timer2::now()
{
    auto now = std::chrono::system_clock::now();

    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    return now_ms.time_since_epoch().count();
}


void Timer2::on_timer()
{
    if (!m_func || m_repeat == 0)
    {
        return;
    }
    m_func();
    m_time += m_period;
    if (m_repeat > 0)
    {
        m_repeat--;
    }
}