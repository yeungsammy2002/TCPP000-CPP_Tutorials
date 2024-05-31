#include "cmd_opt.h"

using namespace regulus::utilities;

CmdOpt::~CmdOpt()
{
    clear();
}

void CmdOpt::add(const string& opt_name, ArgType arg_type, OptType opt_type,
    const src location)
{
    auto file = location.file_name();
    auto line = location.line();
    if ("" == opt_name || opt_name.empty())
    {
        Err::logic("ERROR: option name cannot be empty string - %s:%d", file, line);
    }
    if (!Str::are_alpha(opt_name))
    {
        Err::logic("ERROR: option name contains non-alphabet characters - %s:%d", file, line);
    }
    if (m_opts.find(opt_name) != m_opts.end())
    {
        Err::logic("ERROR: option already exists - %s:%d", file, line);
    }
    struct Opt opt;
    opt.opt_type = opt_type;
    opt.arg_type = arg_type;
    m_opts[opt_name] = opt;
}

bool CmdOpt::is_register(const string& opt_name) const
{
    return m_opts.find(opt_name) != m_opts.end();
}

const CmdOpt::Opt& CmdOpt::get_opt(const string& opt_name, src location) const
{
    auto file = location.file_name();
    auto line = location.line();
    if (!is_register(opt_name))
    {
        Err::logic("ERROR: option \"%s\" does not register - %s:%d", opt_name.c_str(), file, line);
    }
    const auto it = m_opts.find(opt_name);
    return it->second;
}

bool CmdOpt::has(const string& opt_name) const
{
    Opt opt = get_opt(opt_name);
    return nullptr != opt.args;
}

CmdOpt::OptType CmdOpt::get_opt_type(const string& opt_name) const
{
    Opt opt = get_opt(opt_name);
    return opt.opt_type;
}

CmdOpt::ArgType CmdOpt::get_arg_type(const string& opt_name) const
{
    Opt opt = get_opt(opt_name);
    return opt.arg_type;
}

void CmdOpt::clear()
{
    for (auto& pair : m_opts)
    {
        pair.second.opt_type = OPT_OPTIONAL;
        pair.second.arg_type = ARG_NONE;
        delete pair.second.args;
        pair.second.args = nullptr;
    }
}

bool CmdOpt::is_short_opt(const string& str) const
{
    return ("-" == str.substr(0, 1)) && ("--" != str.substr(0, 2));
}

bool CmdOpt::is_long_opt(const string& str) const
{
    return "--" == str.substr(0, 2);
}

void CmdOpt::parse(int argc, const char* const argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        string str = argv[i];
        if ("-" != str.substr(0, 1) && 1 == i)
        {
            struct Opt opt;
            opt.opt_type = OPT_REQUIRED;
            opt.arg_type = ARG_REQUIRED;
            opt.args = new std::vector<string>();
            m_opts["_default"] = opt;
            for (; i < argc; ++i)
            {
                str = argv[i];
                if ("-" == str.substr(0, 1))
                {
                    break;
                }
                m_opts["_default"].args->push_back(str);
            }
        }
        else if (is_short_opt(str))
        {
            if (2 > str.length())
            {
                continue;
            }
            string opt_name = str.substr(1, 1);
            switch (get_arg_type(opt_name))
            {
            case ARG_NONE:
            {
                if (i + 1 < argc)
                {
                    string next_str = argv[i + 1];
                    if ("-" != next_str.substr(0, 1))
                    {
                        Err::logic("ERROR: \"%s\" is no option argument", opt_name.c_str());
                    }
                }

                for (int k = 1; k < str.length(); ++k)
                {
                    string o = string(1, str[k]);
                    if (ARG_NONE != get_arg_type(o))
                    {
                        continue;
                    }
                    m_opts[o].args = new std::vector<string>();
                }
            }
            break;
            case ARG_OPTIONAL:
            {
                m_opts[opt_name].args = new std::vector<string>();
                for (; i < argc; ++i)
                {
                    if (i + 1 == argc)
                    {
                        break;
                    }
                    str = argv[i + 1];
                    if ("-" == str.substr(0, 1))
                    {
                        break;
                    }
                    m_opts[opt_name].args->push_back(str);
                }
            }
            break;
            case ARG_REQUIRED:
            {
                if (i + 1 == argc || "-" == string(argv[i + 1]).substr(0, 1))
                {
                    Err::logic("ERROR: option \"%s\" required argument", opt_name.c_str());
                }
                m_opts[opt_name].args = new std::vector<string>();
                for (; i < argc; ++i)
                {
                    if (i + 1 == argc)
                    {
                        break;
                    }
                    str = argv[i + 1];
                    if ("-" == str.substr(0, 1))
                    {
                        break;
                    }
                    m_opts[opt_name].args->push_back(str);
                }
            }
            break;
            default:
                break;
            }
        }
        else if (is_long_opt(str))
        {
            if (3 > str.length())
            {
                continue;
            }
            string opt_name = str.substr(2);
            switch (get_arg_type(opt_name))
            {
            case ARG_NONE:
            {
                if (i + 1 < argc)
                {
                    if ("-" != string(argv[i + 1]).substr(0, 1))
                    {
                        Err::logic("ERROR: \"%s\" is no option argument", opt_name.c_str());
                    }
                }
                m_opts[opt_name].args = new std::vector<string>();
            }
            break;
            case ARG_OPTIONAL:
            {
                m_opts[opt_name].args = new std::vector<string>();
                for (; i < argc; ++i)
                {
                    if (i + 1 == argc)
                    {
                        break;
                    }
                    str = argv[i + 1];
                    if ("-" == str.substr(0, 1))
                    {
                        break;
                    }
                    m_opts[opt_name].args->push_back(str);
                }
            }
            break;
            case ARG_REQUIRED:
            {
                if (i + 1 == argc || "-" == string(argv[i + 1]).substr(0, 1))
                {
                    Err::logic("ERROR: option \"%s\" required argument", opt_name.c_str());
                }
                m_opts[opt_name].args = new std::vector<string>();
                for (; i < argc; ++i)
                {
                    if (i + 1 == argc)
                    {
                        break;
                    }
                    str = argv[i + 1];
                    if ("-" == str.substr(0, 1))
                    {
                        break;
                    }
                    m_opts[opt_name].args->push_back(str);
                }
            }
            break;
            default:
                break;
            }
        }
    }
}

void CmdOpt::show() const
{
    for (const auto& pair : m_opts)
    {
        if (nullptr == pair.second.args)
        {
            continue;
        }
        if (nullptr != pair.second.args && 0 == pair.second.args->size())
        {
            std::cout << "| " << pair.first << " |";
            continue;
        }
        std::cout << "| " << pair.first << ":" << "[ ";
        for (const auto& str : *(pair.second.args))
        {
            std::cout << str << " ";
        }
        std::cout << "] |";
    }
    std::cout << std::endl;
}