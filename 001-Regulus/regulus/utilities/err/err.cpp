#include "err.h"

using namespace regulus::utilities;

void Err::logic(const char * format, ...)
{
    std::stringstream ss;
    va_list arg_ptr;
    va_start(arg_ptr, format);
    const auto size = std::vsnprintf(nullptr, 0, format, arg_ptr);
    va_end(arg_ptr);

    if (size > 0)
    {
        char * buff = new char[size + 1];
        va_start(arg_ptr, format);
        std::vsnprintf(buff, size + 1, format, arg_ptr);
        va_end(arg_ptr);
        buff[size] = 0;
        ss << buff;
        delete[] buff;
    }
    throw std::logic_error(ss.str());
}