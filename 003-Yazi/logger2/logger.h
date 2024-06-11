#pragma once

#include <string>
#include <fstream>
#include <ctime>            // use ctime instead of time.h
#include <iostream>
#include <sstream>
#include <cstdarg>          // using cstdarg over stdarg.h because it does not maintain anymore

#ifdef WIN32
#include <windows.h>        // windows.h for Sleep()
#else
#include <unistd.h>         // unistd.h for usleep() & sleep(), Windows didn't have this header
#endif

#include <utilities/singleton.h>

using std::string;

namespace demo {
namespace utilities {

//#define log_debug(format, ...)                                                              \
//    Logger::instance()->log(Logger::LOG_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

//#define log_info(format, ...)                                                               \
//    Logger::instance()->log(Logger::LOG_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

//#define log_warn(format, ...)                                                              \
//    Logger::instance()->log(Logger::LOG_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

//#define log_error(format, ...)                                                               \
    Logger::instance()->log(Logger::LOG_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

//#define log_fatal(format, ...)                                                              \
//    Logger::instance()->log(Logger::LOG_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)


#define log_debug(format, ...)                                                                          \
    Singleton<Logger>::instance()->log(Logger::LOG_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_info(format, ...)                                                                           \
    Singleton<Logger>::instance()->log(Logger::LOG_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_warn(format, ...)                                                                           \
    Singleton<Logger>::instance()->log(Logger::LOG_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_error(format, ...)                                                                          \
    Singleton<Logger>::instance()->log(Logger::LOG_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_fatal(format, ...)                                                                          \
    Singleton<Logger>::instance()->log(Logger::LOG_FATAL, __FILE__, __LINE__, format, ##__VA__ARGS__)


class Logger
{
    SINGLETON(Logger);
public:
    enum Level
    {
        LOG_DEBUG = 0,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_FATAL,
        LOG_COUNT
    };

//    static Logger * instance();

    void open(const string & filename);
    void close();

    void set_level(int level);
    void set_max_size(int bytes);
    void set_console(bool console);
    void log(Level level, const char * file, int line, const char * format, ...);

private:
//    Logger();
//    ~Logger();

    void rotate();
    void sleep(int milliseconds);
    void localtime(struct tm * time_info, const std::time_t * ticks);

private:
    string m_filename;
    std::ofstream m_ofs;
    int m_max = 0;
    int m_len = 0;
    int m_level = LOG_DEBUG;
    bool m_console = true;
    static const char * s_level[LOG_COUNT];

//    static Logger * m_instance;
};

}
}