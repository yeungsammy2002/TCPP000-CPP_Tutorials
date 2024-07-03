#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <sstream>
#include <cstdarg>


using std::string;

namespace demo {
namespace utilities {

class String
{
public:
    static string to_lower(string str);
    static string to_upper(string str);

    static string ltrim(string str, const string & trims);
    static string rtrim(string str, const string & trims);
    static string trim(string str, const string & trims);

    static std::vector<string> split(const string & input, const string & separator);
    static std::vector<string> split(const string & input, const char separator);

    static string join(const std::vector<string> & input, const string & separator);
    static string join(const std::vector<string> & input, const char separator);

    static bool has_prefix(const string & input, const string & prefix);
    static bool has_suffix(const string & input, const string & suffix);

    static string capitalize(const string & input);
    static string format(const char * format, ...);

};

}
}