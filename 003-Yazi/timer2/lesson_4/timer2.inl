
template<typename F, typename... Args>
void Timer2::callback(int milliseconds, F && f, Args && ... args)
{
    m_period = milliseconds;
    m_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
}
