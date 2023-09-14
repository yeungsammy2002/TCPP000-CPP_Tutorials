#pragma once

#include <fstream>
#include <ctime>
#include <cstring>
#include <stdarg.h>

#include <iostream>

#include "Singleton.h"

namespace demo {
    namespace utilities {

#define debug(format, ...) \
        Singleton<Logger>::instance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define info(format, ...) \
        Singleton<Logger>::instance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define warn(format, ...) \
        Singleton<Logger>::instance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define error(format, ...) \
        Singleton<Logger>::instance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define fatal(format, ...) \
        Singleton<Logger>::instance()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

        class Logger {
            friend class Singleton<Logger>;

        public:
            enum Level {
                DEBUG = 0,
                INFO,
                WARN,
                ERROR,
                FATAL,
                LEVEL_COUNT
            };

            void open(std::string filename);

            void close();

            void log(Level level, const char * file, int line, const char * format, ...);

            void level(Level level);

            void max(int bytes);

        private:
            Logger() : m_level(DEBUG), m_len(0), m_max(0) {}

            ~Logger() {}

            Logger(const Logger &) = delete;

            Logger & operator=(const Logger &) = delete;

            void rotate();

        private:
            std::ofstream m_fout;
            static const char * s_level[LEVEL_COUNT];
            std::string m_filename;
            Level m_level;
            int m_len;
            int m_max;
        };
    }
}