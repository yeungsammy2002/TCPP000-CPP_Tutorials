#pragma once

#include <map>

#include <utilities/timer2.h>

class TimerManager
{
public:
    TimerManager() = default;
    ~TimerManager() = default;

    template<typename F, typename ... Args>
    void schedule(int milliseconds, F && f, Args && ... args);

    template<typename F, typename ... Args>
    void schedule(int milliseconds, int repeat, F && f, Args && ... args);

    void update();

private:
    std::multimap<int64_t, Timer2> m_timers;
};

#include <utilities/timer_manager.inl>


