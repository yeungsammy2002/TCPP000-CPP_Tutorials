#pragma once

#include <map>

#include <utilities/singleton.h>
#include <timer/timer.h>

using namespace demo::utilities;

namespace demo {
namespace timer {

class TimerManager
{
    SINGLETON(TimerManager);
public:
    template<typename F, typename... Args>
    void schedule(int milliseconds, F && f, Args && ... args);

    template<typename F, typename... Args>
    void schedule(int milliseconds, int repeat, F && f, Args && ... args);

    void update();

private:
    std::multimap<int64_t, Timer> m_timers;
};

#include <timer/timer_manager.inl>

}
}