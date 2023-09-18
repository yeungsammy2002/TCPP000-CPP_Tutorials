#include "Logger.h"

using namespace demo::utilities;

const char * Logger::s_level[LEVEL_COUNT] {
        "DEBUG",
        "INFO",
        "WARNING",
        "ERROR",
        "FATAL ERROR"
};

void Logger::open(const std::string& filename) {
    m_filename = filename;
    m_fout.open(filename, std::ios::app);
    if (m_fout.fail())
        throw std::logic_error("failed to open " + m_filename);
    m_fout.seekp(0, std::ios::end);
    m_len = m_fout.tellp();
}

void Logger::close() {
    m_fout.close();
}

void Logger::log(Level level, const char * file, int line, const char * format, ...) {
    if (m_level > level) return;
    if (m_fout.fail())
        throw std::logic_error("failed to open " + m_filename);

//    std::cout << s_level[level] << std::endl;
//    std::cout << file << std::endl;
//    std::cout << line << std::endl;

    std::time_t ticks = std::time(NULL);
    std::tm * ptm = std::localtime(&ticks);

    char timestamp[64];
    std::memset(timestamp, 0, sizeof(timestamp));
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d(%a) %H:%M:%S GMT+8", ptm);

//    std::cout << timestamp << std::endl;

    const char * fmt = "%s [%s] %s:%d ";
    int size = std::snprintf(NULL, 0, fmt, timestamp, s_level[level], file, line);

    if (size > 0) {
        char * buffer = new char[size + 1];
        std::snprintf(buffer, size + 1, fmt, timestamp, s_level[level], file, line);
        buffer[size] = 0;
        m_len += size;

//        std::cout << buffer << std::endl;

        m_fout << buffer;
        delete[] buffer;
    }

    va_list arg_ptr;
    va_start(arg_ptr, format);
    size = std::vsnprintf(NULL, 0, format, arg_ptr);
    va_end(arg_ptr);

    if (size > 0) {
        char * content = new char[size + 1];
        va_start(arg_ptr, format);
        std::vsnprintf(content, size + 1, format, arg_ptr);
        va_end(arg_ptr);
        content[size] = 0;
        m_len += size;

//        std::cout << content << std::endl;

        m_fout << content;
        delete[] content;
    }

    m_fout << "\n";
    m_fout.flush();

    if (m_len > m_max && m_max > 0)
        rotate();
}

void Logger::level(Level level) {
    m_level = level;
}

void Logger::max(int bytes) {
    m_max = bytes;
}

void Logger::rotate() {
    close();

    char timestamp[32];
    std::memset(timestamp, 0, sizeof(timestamp));

    std::time_t ticks = std::time(NULL);
    std::tm * ptm = std::localtime(&ticks);
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", ptm);

//    std::cout << timestamp << std::endl;

    std::string filename = m_filename + timestamp;
    if (std::rename(m_filename.c_str(), filename.c_str()) != 0)
        throw std::logic_error("failed to rename log file: " + std::string(std::strerror(errno)));
    open(m_filename);
}