#include <utilities/timer_manager.h>





void TimerManager::update()
{
    if (m_timers.empty())
    {
        return;
    }
    int64_t now = Timer2::now();
    for (auto it = m_timers.begin(); it != m_timers.end();)
    {
        if (it->first > now)
        {
            return;
        }
        it->second.on_timer();

        Timer2 t = it->second;
        it = m_timers.erase(it);
        if (t.m_repeat != 0)
        {
            auto new_it = m_timers.insert(std::make_pair(t.m_time, t));
            if (it == m_timers.end() || new_it->first < it->first)
            {
                it = new_it;
            }
        }
    }
}
