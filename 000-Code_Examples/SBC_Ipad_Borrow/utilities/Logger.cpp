#include "Logger.h"

using namespace regulus::utilities;

Logger::Logger() {}

Logger::~Logger()
{
	//m_fout << "close\n";
	close();
}

const char * Logger::s_level[L_LEVEL_COUNT]{
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
};

bool Logger::open(const string & filename)
{
	m_filename = filename;
	m_fout.open(filename, std::ios::app);

	if (m_fout.fail())
		return false;

	return true;
}

void Logger::close()
{
	m_fout.close();
}

void Logger::log(Level level, const char * file, int line, const char * format, ...)
{
	if (m_level > level)
		return;

	const time_t ticks = std::time(NULL);
	long long t = ticks;

	struct tm * ptm = new std::tm();
	localtime_s(ptm, &ticks);
	char timestamp[64]{ 0 };
	std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d(%a) %H:%M:%S GMT+8", ptm);

	const char * fmt = "%s [%s] %s:%d ";
	int size = std::snprintf(NULL, 0, fmt, timestamp, s_level[level], file, line);

	if (0 < size)
	{
		char * buff = new char[size + 1];
		std::snprintf(buff, size + 1, fmt, timestamp, s_level[level], file, line);
		buff[size] = 0;
		m_fout << buff;
		delete[] buff;
	}

	va_list arg_ptr;
	va_start(arg_ptr, format);
	int va_size = std::vsnprintf(NULL, 0, format, arg_ptr);
	va_end(arg_ptr);

	if (0 < va_size)
	{
		char * buff = new char[va_size + 1];
		va_start(arg_ptr, format);
		std::vsnprintf(buff, va_size + 1, format, arg_ptr);
		va_end(arg_ptr);
		buff[va_size] = 0;
		m_fout << buff;
		delete[] buff;
	}

	m_fout << '\n';
	m_fout.flush();

	delete ptm;
	ptm = nullptr;
}

void Logger::level(Level level)
{
	m_level = level;
}