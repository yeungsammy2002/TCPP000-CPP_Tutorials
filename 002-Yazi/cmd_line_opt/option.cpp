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

void Option::parse(int argc, char * const argv[])
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
            const auto pos = str.find_first_of('=');
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
            string opt = arg.substr(1, 1);
            switch (type(opt))
            {
                case OPT_NO:
                {
                    for (int k = 1; k < arg.length(); ++k)
                    {
                        string o = string(1, arg[k]);
                        if (OPT_NO != type(o))
                        {
                            continue;
                        }
                        m_args[o] = "";
                    }
                }
                    break;
                case OPT_REQUIRED:
                    if (2 < arg.length())
                    {
                        m_args[opt] = arg.substr(2);
                        break;
                    } else
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
                    string val = arg.substr(2);
                    m_args[opt] = val;
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
    for (const auto & pair: m_args)
    {
        std::cout << pair.first << ":" << pair.second << " ";
    }
    std::cout << std::endl;
}

bool Option::has(const string & opt)
{
    return m_opts.find(opt) != m_opts.end();
}

bool Option::get_bool(const string & opt)
{
    if (!has(opt))
    {
        return false;
    }
    return m_args.find(opt) != m_args.end();
}

int Option::get_int(const string & opt)
{
    if (!has(opt) || m_args.find(opt) == m_args.end())
    {
        return 0;
    }
    std::stringstream ss;
    ss << m_args[opt];
    int value = 0;
    ss >> value;
    return value;
}

string Option::get_string(const string & opt)
{
    if (!has(opt) || m_args.find(opt) == m_args.end())
    {
        return "";
    }
    return m_args[opt];
}