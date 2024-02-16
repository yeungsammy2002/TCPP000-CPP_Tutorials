#include "Logger.h"

using namespace regulus::utilities;

Logger::Logger() : m_level(L_DEBUG), m_max(0), m_len(0) {}

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

bool Logger::open(const string & folder_path)
{
	m_folder_path = folder_path;
	m_filename = folder_path + "log.txt";
	m_fout.open(m_filename, std::ios::app);

	if (m_fout.fail())
		return false;

	m_fout.seekp(0, std::ios::end);
	m_len = m_fout.tellp();

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

	struct std::tm * ptm = std::localtime(&ticks);
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
		m_len += size;
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
		m_len += va_size;
		delete[] buff;
	}

	m_fout << '\n';
	m_fout.flush();

	if (m_len > m_max && m_max > 0)
	{
		rotate();
	}
}

void Logger::level(Level level)
{
	m_level = level;
}

void Logger::max(const int bytes)
{
	if (bytes < 10240)
		throw std::logic_error("log file must have at least 10240 btyes");

	m_max = bytes;
}

void Logger::rotate()
{
	close();

	std::time_t ticks = std::time(NULL);
	struct std::tm * ptm = std::localtime(&ticks);
	char timestamp[32]{ 0 };
	std::strftime(timestamp, sizeof(timestamp), "%Y-%d-%m_%H-%M-%S", ptm);
	string filename = m_folder_path + "log_" + timestamp + ".txt";

	if (0 != std::rename(m_filename.c_str(), filename.c_str()))
		log_error("failed to rename log file: %s", filename.c_str());

	open(m_folder_path);
}