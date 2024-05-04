#include <utilities/logger.h>

using namespace demo::utilities;

const char * Logger::s_level[LEVEL_COUNT]{
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL"
};

Logger::Logger() : m_level(Logger::DEBUG), m_len(0), m_max(0) {}

Logger::~Logger()
{
    close();
}

void Logger::open(const string & filename)
{
    m_filename = filename;
    m_fout.open(filename, std::ios::app);
    if (m_fout.fail())
    {
        throw std::logic_error("failed to open file: " + filename);
    }
    m_fout.seekp(0, std::ios::end);
    m_len = m_fout.tellp();
}

void Logger::close()
{
    m_fout.close();
}

void Logger::log(Level level, const char * file, int line, const char * format, ...)
{
    if (m_level > level)
    {
        return;
    }
    std::time_t ticks = std::time(NULL);
    struct tm * ptm = std::localtime(&ticks);
    char timestamp[64]{ 0 };
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d(%a) %H:%M:%S GMT+8", ptm);

//    std::cout << timestamp << std::endl;

    const char * fmt = "%s [%s] %s:%d ";
    const int size = std::snprintf(nullptr, 0, fmt, timestamp, s_level[level], file, line);
    if (size > 0)
    {
        char * buff = new char[size + 1];
        std::snprintf(buff, size + 1, fmt, timestamp, s_level[level], file, line);
        buff[size] = 0;

//        std::cout << buff << std::endl;
        m_fout << buff;
        m_len += size;

        delete[] buff;
    }

    va_list arg_ptr;
    va_start(arg_ptr, format);
    const int va_size = std::vsnprintf(nullptr, 0, format, arg_ptr);
    va_end(arg_ptr);

    if (va_size > 0)
    {
        char * buff = new char[va_size + 1];
        va_start(arg_ptr, format);
        std::vsnprintf(buff, va_size + 1, format, arg_ptr);
        va_end(arg_ptr);
        buff[va_size] = 0;

//        std::cout << buff << std::endl;
        m_fout << buff;
        m_len += va_size;

        delete[] buff;
    }

    m_fout << "\n";
    m_fout.flush();

    if (0 < m_max && m_len >= m_max)
    {
        rotate();
    }
}

void Logger::rotate()
{
    close();
    std::time_t ticks = std::time(NULL);
    struct tm * ptm = std::localtime(&ticks);
    char timestamp[32]{ 0 };
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", ptm);
    string filename = m_filename + timestamp;
    if (0 != std::rename(m_filename.c_str(), filename.c_str()))
    {
        throw std::logic_error("rename file failed: " + filename);
    }
    open(m_filename);
}

