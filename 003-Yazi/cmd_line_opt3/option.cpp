#include <utilities/option.h>

using namespace demo::utilities;





void Option::add(const string & opt, Type type)
{
    m_opts[opt] = type;
}





Option::Type Option::type(const string & opt) const
{
    const auto it = m_opts.find(opt);
    assert(it != m_opts.end() && "option must be registered");
    return it->second;
}





void Option::parse(const int argc, const char * argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i];
        if ("-" != arg.substr(0, 1))
        {
            continue;
        } else if (1 == arg.length())
        {
            continue;
        } else if ("-" == arg.substr(0, 1) && "-" != arg.substr(1, 1))
        {
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
                default:
                {
                    throw std::logic_error("option is not registed");
                }
                    break;
            }
        } else if ("--" == arg.substr(0, 2))
        {
            string str = arg.substr(2);
            const auto pos = str.find("=");
            if (pos != std::string::npos)
            {
                string opt = str.substr(0, pos);
                switch (type(opt))
                {
                    case OPT_NO:
                    {
                        throw std::logic_error("no argument option: " + opt);
                    }
                        break;
                    case OPT_OPTIONAL:
                    case OPT_REQUIRED:
                    {
                        string val = str.substr(pos + 1);
                        m_args[opt] = val;
                    }
                        break;
                    default:
                    {
                        throw std::logic_error("option is not registered");
                    }
                        break;
                }
            } else       // (pos == std::string::npos)
            {
                string opt = str;
                switch (type(opt))
                {
                    case OPT_NO:
                    case OPT_OPTIONAL:
                    {
                        m_args[opt] = "";
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
                    default:
                    {
                        throw std::logic_error("option is not registered");
                    }
                        break;
                }
            }
        }
    }
}





bool Option::has(const string & opt) const
{
    const auto it = m_opts.find(opt);
    assert(it != m_opts.end() && "option must be registered");
    return m_args.end() != m_args.find(opt);
}

Value Option::get(const string & opt)
{
    const auto it = m_opts.find(opt);
    assert(it != m_opts.end() && "option must be registered");
    switch (it->second)
    {
        case OPT_NO:
        {
            return Value(m_args.end() != m_args.find(opt));
        }
            break;
        case OPT_OPTIONAL:
        case OPT_REQUIRED:
        {
            return Value(m_args[opt]);
        }
        default:
            break;
    }
}





void Option::show() const
{
    for (const auto & pair: m_args)
    {
        std::cout << pair.first << ":" << pair.second << std::endl;
    }
}