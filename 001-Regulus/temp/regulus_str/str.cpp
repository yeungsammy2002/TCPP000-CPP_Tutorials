#include "regulus_str\utilities\str.h"

using namespace regulus::utilities;

bool Str::are_alpha(const string& str)
{
    for (int i = 0; i < str.length(); ++i)
    {
        if (0 == isalpha(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool Str::are_digit(const string& str)
{
    for (int i = 0; i < str.length(); ++i)
    {
        if (0 == isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool Str::are_alnum(const string& str)
{
    for (int i = 0; i < str.length(); ++i)
    {
        if (0 == isalnum(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool Str::contain(const string& str, const string& chars)
{
    for (int i = 0; i < chars.size(); ++i)
    {
        if (std::string::npos != str.find(chars[i]))
        {
            return true;
        }
    }
    return false;
}

string Str::trim(string str)
{
    str.erase(0, str.find_first_not_of(" \n\r"));
    str.erase(str.find_last_not_of(" \n\r") + 1);
    return str;
}

string Str::trim(string str, const string& chars_to_trim)
{
    str.erase(0, str.find_first_not_of(chars_to_trim));
    str.erase(str.find_last_not_of(chars_to_trim) + 1);
    return str;
}

string Str::to_str(const char* format, ...)
{
    std::stringstream ss;
    va_list arg_ptr;
    va_start(arg_ptr, format);
    const auto size = std::vsnprintf(nullptr, 0, format, arg_ptr);
    va_end(arg_ptr);

    if (size > 0)
    {
        char* buff = new char[size + 1];
        va_start(arg_ptr, format);
        std::vsnprintf(buff, size + 1, format, arg_ptr);
        va_end(arg_ptr);
        buff[size] = 0;
        ss << buff;
        delete[] buff;
    }

    return ss.str();
}