#pragma once

#include <ctime>
#include <iostream>
#include <string>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

using std::string;

namespace demo {
namespace utilities {

class Time
{
public:
    Time();
    ~Time() = default;

    int year() const;
    int month() const;
    int day() const;
    int hour() const;
    int minute() const;
    int second() const;
    int week() const;

    void show() const;

    double seconds() const;
    double milliseconds() const;
    double operator-(const Time & other) const;

    static void sleep(int milliseconds);

    string format(const string & format) const;

private:
    void get_local_time(struct std::tm * tm, const std::time_t * ticks);
    void get_time_of_day(struct timeval * tv);

private:
    struct tm m_tm = { 0 };
    int m_sec;
    int m_usec;
};

}
}