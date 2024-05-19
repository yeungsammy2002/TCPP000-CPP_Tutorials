#pragma once

#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <stdarg.h>

#include <utilities/singleton.h>

using std::string;

namespace demo {
namespace utilities {

#define log_debug(format, ...)   \
    Singleton<Logger>::instance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_info(format, ...) \
    Singleton<Logger>::instance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_warn(format, ...)   \
    Singleton<Logger>::instance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_error(format, ...)  \
    Singleton<Logger>::instance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_fatal(format, ...)  \
    Singleton<Logger>::instance()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

class Logger
{
    SINGLETON(Logger);

public:
    enum Level
    {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LEVEL_COUNT
    };

    void open(const string & filename);

    void close();

    void log(Level level, const char * file, int line, const char * format, ...);

    void level(Level level) { m_level = level; }

    void rotate();

    void max(int bytes) { m_max = bytes; }

private:
    string m_filename;
    std::ofstream m_fout;
    static const char * s_level[LEVEL_COUNT];
    Level m_level;
    int m_len;
    int m_max;
};

}
}