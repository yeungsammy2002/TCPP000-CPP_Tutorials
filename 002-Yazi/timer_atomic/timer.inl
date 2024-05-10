template<typename F, typename... Args>
void Timer::start(int milliseconds, F && f, Args && ...args)
{
    if (m_active.load())
    {
        return;
    }
    m_period = milliseconds;
    m_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    m_active.store(true);
    m_thread = std::thread([&]() {
        if (0 > m_repeat)
        {
            while (m_active.load())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(m_period));
                if (!m_active.load())
                {
                    return;
                }
                m_func();
            }
        } else
        {
            while (0 < m_repeat)
            {
                if (!m_active.load())
                {
                    return;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(m_period));
                if (!m_active.load())
                {
                    return;
                }
                m_func();
                --m_repeat;
            }
        }
    });
    m_thread.detach();
}

