#include <utilities/option.h>

using namespace demo::utilities;

void Option::add(const string & opt, Type type)
{
    m_opts[opt] = type;
}

Option::Type Option::type(const string & opt) const
{
    const auto it = m_opts.find(opt);
    if (it == m_opts.end())
    {
        return OPT_UNKNOWN;
    }
    return it->second;
}

void Option::parse(int argc, char * argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        if ("-" != arg.substr(0, 1))
        {
            continue;
        }
        if ("--" == arg.substr(0, 2))
        {
            string str = arg.substr(2);
            const auto pos = str.find_first_of("=");
            if (pos != std::string::npos)
            {
                string opt = str.substr(0, pos);
                string val = str.substr(pos + 1);
                switch (type(opt))
                {
                    case OPT_NO:
                    {
                        throw std::logic_error("no argument option: " + opt);
                    }
                        break;
                    case OPT_REQUIRED:
                    case OPT_OPTIONAL:
                    {
                        m_args[opt] = val;
                    }
                        break;
                    default:
                        break;
                }
            } else
            {
                switch (type(str))
                {
                    case OPT_NO:
                    case OPT_OPTIONAL:
                    {
                        m_args[str] = "";
                    }
                        break;
                    case OPT_REQUIRED:
                    {
                        if (i + 1 >= argc)
                        {
                            throw std::logic_error("option require argument: " + str);
                        }
                        string val = argv[i + 1];
                        if ("-" == val.substr(0, 1))
                        {
                            throw std::logic_error("option missing argument: " + str);
                        }
                        m_args[str] = val;
                        ++i;
                    }
                        break;
                    default:
                        break;
                }
            }
        } else
        {
            if (1 == arg.length())
            {
                continue;
            }
            string opt = arg.substr(1, 1);
            switch (type(opt))
            {
                case OPT_NO:
                {
                    for (int k = 1; k < arg.length(); ++k)
                    {
                        string o(1, arg[k]);
                        if (OPT_NO != type(o))
                        {
                            continue;
                        }
                        m_args[o] = "";
                    }
                }
                    break;
                case OPT_REQUIRED:
                {
                    if (i + 1 >= argc)
                    {
                        throw std::logic_error("option require argument: " + opt);
                    }
                    string val = argv[i + 1];
                    if ("-" == val.substr(0, 1))
                    {
                        throw std::logic_error("option missing argument: " + opt);
                    }
                    m_args[opt] = val;
                    ++i;
                }
                    break;
                case OPT_OPTIONAL:
                {
                    if (2 < arg.length())
                    {
                        m_args[opt] = arg.substr(2);
                    } else
                    {
                        m_args[opt] = "";
                    }
                }
                    break;
                default:
                    break;
            }
        }
    }
}

void Option::show() const
{
    for (auto it = m_args.begin(); it != m_args.end(); ++it)
    {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << std::endl;
}

bool Option::has(const string & opt) const
{
    return m_opts.find(opt) != m_opts.end();
}

bool Option::get_bool(const string & opt) const
{
    return m_args.find(opt) != m_args.end();
}

int Option::get_int(const string & opt) const
{
    const auto it = m_args.find(opt);
    if (it == m_args.end())
    {
        return 0;
    }
    std::stringstream ss;
    int value = 0;
    ss << it->second;
    ss >> value;
    return value;
}

string Option::get_string(const string & opt) const
{
    const auto it = m_args.find(opt);
    if (it == m_args.end())
    {
        return "";
    }
    return it->second;
}
