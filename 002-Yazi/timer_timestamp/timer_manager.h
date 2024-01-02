#pragma once

#include <map>
#include <cstdint>

#include <timer/timer.h>
#include <utilities/singleton.h>

using namespace demo::utilities;

namespace demo {
namespace timer {

class TimerManager
{
    SINGLETON(TimerManager);
public:
//// constructor and destructor cannot exist when using Singleton
//    TimerManager() = default;
//
//    ~TimerManager() = default;

    template<typename F, typename... Args>
    void schedule(int milliseconds, F && f, Args && ... args);

    template<typename F, typename... Args>
    void schedule(int milliseconds, int repeat, F && f, Args && ... args);

    void update();

private:
    std::multimap<int64_t, Timer> m_timers;
};

template<typename F, typename... Args>
void TimerManager::schedule(int milliseconds, F && f, Args && ... args)
{
    schedule(milliseconds, -1, std::forward<F>(f), std::forward<Args>(args)...);
}

template<typename F, typename... Args>
void TimerManager::schedule(int milliseconds, int repeat, F && f, Args && ...args)
{
    Timer t(repeat);
    t.callback(milliseconds, std::forward<F>(f), std::forward<Args>(args)...);
    m_timers.insert(std::make_pair(t.m_time, t));
}

}
}