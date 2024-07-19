#include <utilities/string.h>

using namespace demo::utilities;





string String::to_lower(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

string String::to_upper(string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}





string String::ltrim(string str, const string & trims)
{
    const size_t pos = str.find_first_not_of(trims);
    if (pos == std::string::npos)
    {
        str.clear();
    } else
    {
        str.erase(0, pos);
    }
    return str;
}

string String::rtrim(string str, const string & trims)
{
    const size_t pos = str.find_last_not_of(trims);
    if (pos == std::string::npos)
    {
        str.clear();
    } else
    {
        str.erase(pos + 1);
    }
    return str;
}

string String::trim(string str, const string & trims)
{
    str = ltrim(str, trims);
    str = rtrim(str, trims);
    return str;
}





std::vector<string> String::split(const string & s, const string & separators)
{
    std::vector<string> output;
    size_t start = 0;
    size_t sp = s.find_first_of(separators, start);

    while (sp != std::string::npos)
    {
        if (sp != start)
        {
            output.push_back(s.substr(start, sp - start));
        }

        start = sp + 1;
        sp = s.find_first_of(separators, start);
    }

    if (start == 0)
    {
        if (s.size() != 0)
        {
            output.push_back(s);
        }
    } else
    {
        sp = s.find_last_of(separators);
        if (sp != s.size() - 1)
        {
            output.push_back(s.substr(start, s.size() - start));
        }
    }

    return output;
}

std::vector <string> String::split(const string & input, const char separator)
{
    return split(input, string(1, separator));
}





string String::join(const std::vector <string> & input, const string & separator)
{
    std::ostringstream oss;
    for (auto it = input.begin(); it != input.end(); ++it)
    {
        if (it != input.begin())
        {
            oss << separator;
        }
        oss << *it;
    }
    return oss.str();
}

string String::join(const std::vector <string> & input, const char separator)
{
    return join(input, string(1, separator));
}





bool String::has_prefix(const string & input, const string & prefix)
{
    if (input.size() < prefix.size())
    {
        return false;
    }
    return input.substr(0, prefix.size()) == prefix;
}

bool String::has_suffix(const string & input, const string & suffix)
{
    if (input.size() < suffix.size())
    {
        return false;
    }
    return input.substr(input.size() - suffix.size(), suffix.size()) == suffix;
}





string String::capitalize(const string & input)
{
    string str = input;
    if (str.empty())
    {
        return str;
    }
    char ch = input[0];
    if (::islower(ch))
    {
        ch = (char) ::toupper(ch);
        std::replace(str.begin(), str.begin() + 1, str[0], ch);     // defined in <algorithm>
    }
    return str;
}

string String::format(const char * format, ...)
{
    std::ostringstream oss;
    va_list arg_ptr;
    va_start(arg_ptr, format);
    const int len = std::vsnprintf(nullptr, 0, format, arg_ptr);
    va_end(arg_ptr);
    if (len > 0)
    {
        char * buf = new char[len + 1];
        va_start(arg_ptr, format);
        std::vsnprintf(buf, len + 1, format, arg_ptr);
        va_end(arg_ptr);
        buf[len] = 0;
        oss << buf;
        delete[] buf;
    }
    return oss.str();
}