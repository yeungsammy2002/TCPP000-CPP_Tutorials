#pragma once

#include <string>
#include <cctype>
#include <stdarg.h>
#include <sstream>

using std::string;

namespace regulus {
namespace utilities {

class Str
{
public:
    static bool are_alpha(const string & str);

    static bool are_digit(const string & str);

    static bool are_alnum(const string & str);

    static bool contain(const string & str, const string & chars);

    static string trim(string str);

    static string trim(string str, const string & chars_to_trim);

    static string to_str(const char * format, ...);
};

}
}