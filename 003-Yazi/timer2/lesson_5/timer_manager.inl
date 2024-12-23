
template<typename F, typename ... Args>
void TimerManager::schedule(int milliseconds, F && f, Args && ... args)
{
    schedule(milliseconds, -1, std::forward<F>(f), std::forward<Args>(args)...);
}

template<typename F, typename ... Args>
void TimerManager::schedule(int milliseconds, int repeat, F && f, Args && ... args)
{
    Timer2 t(repeat);
    t.callback(milliseconds, std::forward<F>(f), std::forward<Args>(args)...);
    m_timers.insert(std::make_pair(t.m_time, t));
}

