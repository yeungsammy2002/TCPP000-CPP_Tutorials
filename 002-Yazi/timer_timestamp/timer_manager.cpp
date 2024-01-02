#include <timer/timer_manager.h>

using namespace demo::timer;

void TimerManager::update()
{
    if (m_timers.empty())
        return;

    int64_t now = Timer::now();

    for (auto it = m_timers.begin(); it != m_timers.end();)
    {
        if (it->first > now)
            return;

        (it->second).on_timer();
        Timer t = it->second;
        it = m_timers.erase(it);

        if (0 != t.m_period)
        {
            auto new_it = m_timers.insert(std::make_pair(t.m_time, t));

            if (m_timers.end() == it || new_it->first < it->first)
                it = new_it;
        }
    }
}