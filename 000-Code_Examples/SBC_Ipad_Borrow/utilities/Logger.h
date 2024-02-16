#pragma once

#include <string>
#include <fstream>
#include <ctime>
#include <stdarg.h>

#include "Singleton.h"

using std::string;

namespace regulus {
namespace utilities {

#define log_debug(format, ...)		\
Singleton<Logger>::instance()->log(Logger::L_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_info(format, ...)		\
Singleton<Logger>::instance()->log(Logger::L_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_warn(format, ...)		\
Singleton<Logger>::instance()->log(Logger::L_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_error(format, ...)		\
Singleton<Logger>::instance()->log(Logger::L_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define log_fatal(format, ...)		\
Singleton<Logger>::instance()->log(Logger::L_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

class Logger
{
	SINGLETON(Logger);
public:
	enum Level
	{
		L_DEBUG = 0,
		L_INFO,
		L_WARN,
		L_ERROR,
		L_FATAL,
		L_LEVEL_COUNT
	};

	bool open(const string & folder_path);

	void close();

	void log(Level level, const char * file, int line, const char * format, ...);

	void level(Level level);

	void max(const int bytes);

	void rotate();

	string m_folder_path;
private:
	string m_filename;
	std::ofstream m_fout;
	static const char * s_level[L_LEVEL_COUNT];
	Level m_level = L_DEBUG;
	int m_max;
	int m_len;
};

}
}


