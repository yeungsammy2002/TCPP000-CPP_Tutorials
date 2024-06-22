#pragma once

#include <string>
#include <map>
#include <cassert>
#include <stdexcept>

#include <utilities/value.h>

using std::string;

namespace demo {
namespace utilities {

class Option
{
public:
    enum Type
    {
        OPT_NO = 0,
        OPT_OPTIONAL,
        OPT_REQUIRED
    };

    Option() = default;
    ~Option() = default;

    void add(const string & opt, Type type);

    Type type(const string & opt) const;

    void parse(const int argc, const char * argv[]);

    bool has(const string & opt) const;
    Value get(const string & opt);

    void show() const;
    
private:
    std::map<string, Type> m_opts;
    std::map<string, string> m_args;
};

}
}