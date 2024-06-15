#include <iostream>

#include <ctime>
#include <unistd.h>
#include <windows.h>

#include <sys/time.h>

int main()
{
    std::time_t ticks = time(nullptr);      // time_t time(time_t * timer);
    std::printf("%lld\n\n", ticks);         // ticks is actually a `unsigned long` a unix time

    //      struct tm {
    //          int tm_sec;
    //          int tm_min;
    //          int tm_hour;
    //          int tm_mday;
    //          int tm_mon;
    //          int tm_year;
    //          int tm_wday;
    //          int tm_yday;
    //          int tm_isdst;
    //      }
    struct tm * info;

    // struct tm * localtime(const time_t * timer);         in <ctime>
    info = std::localtime(&ticks);
    // note that std::localtime is NOT thread safe
    // the second thread override the first thread returned pointer `info`
    // for Linux, localtime_r() is thread safe
    // for Windows, localtime_s() is thread safe
    // both localtime_r() and localtime_s() are in <ctime>

    std::cout << info->tm_year << std::endl;            // years from 1900
    std::cout << info->tm_year + 1900 << "\n\n";        // readable years

    std::cout << info->tm_mon << std::endl;             // month, ranged from 0 to 11
    std::cout << info->tm_mon + 1 << "\n\n";            // readable month

    std::cout << info->tm_mday << "\n\n";               // month day, ranged from 1 to 31

    std::cout << info->tm_wday << "\n\n";               // weekday, ranged from 0 to 6, 0 means Sunday

    std::cout << info->tm_hour << "\n\n";               // hours, ranged from 0 to 23

    std::cout << info->tm_min << "\n\n";                // minutes, ranged from 0 to 59

    std::cout << info->tm_sec << "\n\n";                // seconds, ranged from 0 to 59

    std::cout << info->tm_yday << "\n\n";               // year day, ranged from 0 to 365

    std::cout << info->tm_isdst << "\n\n";              // daylight time




    // std::asctime() return a readable time in char * format quicky:
    // char * asctime(const struct tm * timeptr)
    printf("%s", std::asctime(info));
    // std::asctime() also NOT thread safe, inside the function there has a char array stored in `static`
    // so it could return a char *. As the first thread created the string,
    // the second thread may modify the created string.

    // std::strftime() create a custom form of a readable time:
    // size_t strftime(char * str, size_t len, const char * format, const struct tm * timeptr);
    char str[32] = { 0 };
    std::strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", info);
    printf("%s\n", str);
    // std::strftime() is cross-platform and thread safe


    // sleep() delay the execution of statements for Linux, in <unistd.h>:
    // unsigned int sleep(unsigned int seconds);
    sleep(1);


    // usleep() delay the execution of statements in microseconds for Linux, in <unistd.h>:
    // void usleep(int micro_seconds);
    usleep(1000 * 1000);


    // Sleep() delay the execution of statements in microseconds for Windows, in <windows.h>:
    // void Sleep(DWORD dwMilliseconds);
    Sleep(1000);

    //      struct timeval {
    //          long tv_sec;
    //          long tv_usec;
    //      }
    struct timeval tv;


    // gettimeofday() return time in milliseconds for recording time used for program execution, in <sys/time.h>:
    // int gettimeofday(struct timeval * tv, struct timezone * tz);     timezone usually be `nullptr`
    gettimeofday(&tv, nullptr);
    std::printf("sec=%ld, usec=%ld", tv.tv_sec, tv.tv_usec);
    // gettimeofday() is NOT Windows supported


    return 0;
}
