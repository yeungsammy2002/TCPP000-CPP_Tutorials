#include <utilities/timer.h>

using namespace demo::utilities;

Timer::Timer() : m_active(false), m_period(0), m_repeat(-1) {}

Timer::Timer(int repeat) : m_active(false), m_period(0), m_repeat(repeat) {}

void Timer::stop()
{
    m_active.store(false);
}

Timer::~Timer()
{
    stop();
}