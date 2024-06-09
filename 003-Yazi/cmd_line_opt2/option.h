#pragma once

#include <map>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>

#include <utilities/value.h>

using std::string;

namespace demo {
namespace utilities {

class Option
{
public:
    enum Type
    {
        OPT_UNKNOWN = 0,
        OPT_NO,
        OPT_REQUIRED,
        OPT_OPTIONAL
    };

    Option() = default;
    ~Option() = default;

    void add(const string & opt, Type type);
    Type type(const string & opt) const;

    void parse(int argc, char * argv[]);

    bool has(const string & opt) const;

//    bool get_bool(const string & opt) const;
//    int get_int(const string & opt) const;
//    string get_string(const string & opt) const;
    Value get(const string & opt);

    void show() const;

private:
    std::map<string, Type> m_opts;
    std::map<string, string> m_args;
};

}
}