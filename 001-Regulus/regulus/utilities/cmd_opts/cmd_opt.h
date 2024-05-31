#pragma once

#include <string>
#include <vector>
#include <map>
#include <experimental/source_location>
#include <iostream>

#include "../str/str.h"
#include "../err/err.h"

using std::string;

namespace regulus {
namespace utilities {

typedef std::experimental::source_location src;

class CmdOpt
{
public:
    enum OptType
    {
        OPT_OPTIONAL = 0,
        OPT_REQUIRED
    };

    enum ArgType
    {
        ARG_NONE = 0,
        ARG_OPTIONAL,
        ARG_REQUIRED,
    };

    CmdOpt() = default;

    ~CmdOpt();

    void add(const string & opt_name, ArgType arg_type, OptType opt_type = OPT_OPTIONAL,
             const src location = src::current());

    bool is_register(const string & opt_name) const;

    bool has(const string & opt_name) const;

    OptType get_opt_type(const string & opt_name) const;

    ArgType get_arg_type(const string & opt_name) const;

    void clear();

    bool is_short_opt(const string & str) const;

    bool is_long_opt(const string & str) const;

    void parse(int argc, const char * const argv[]);

    void show() const;

private:
    struct Opt
    {
        OptType opt_type = OPT_OPTIONAL;
        ArgType arg_type = ARG_NONE;
        std::vector<string> * args = nullptr;
    };

    const CmdOpt::Opt & get_opt(const string & opt_name,
                                src location = src::current()) const;

private:
    std::map<string, Opt> m_opts;
};

}
}