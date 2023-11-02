#include <utilities/option.h>

using namespace demo::utilities;

void Option::add(const string & opt, Type type)
{
    m_opts[opt] = type;
}

Option::Type Option::type(const string & opt)
{
    auto it = m_opts.find(opt);
    if (it == m_opts.end())
        return OPT_UNKNOWN;
    return it->second;
}

void Option::parse(int argc, char * argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        string arg = argv[i];
        string opt;
        string val;
        if ("--" == arg.substr(0, 2))
        {
            string str = arg.substr(2);
            auto pos = str.find('=');
            if (string::npos != pos)
            {
                opt = str.substr(0, pos);
                val = str.substr(pos + 1);
                switch (type(opt))
                {
                    case OPT_NONE:
                        throw std::logic_error("no argument option: " + opt);
                    case OPT_OPTIONAL:
                    case OPT_REQUIRED:
                    {
                        m_args[opt] = val;
                        break;
                    }
                    default:
                        break;
                }
            } else
            {
                opt = str;
                switch (type(opt))
                {
                    case OPT_NONE:
                    case OPT_OPTIONAL:
                    {
                        m_args[opt] = "";
                        break;
                    }
                    case OPT_REQUIRED:
                    {
                        if (i + 1 >= argc)
                            throw std::logic_error("option require argument: " + opt);
                        val = argv[i + 1];
                        if ("-" == val.substr(0, 1))
                            throw std::logic_error("option missing argument: " + opt);
                        m_args[opt] = val;
                        ++i;
                        break;
                    }
                    default:
                        break;
                }
            }
        } else if ("-" == arg.substr(0, 1))
        {
            opt = arg.substr(1, 1);
            switch (type(opt))
            {
                case OPT_NONE:
                {
                    if (arg.length() >= 2)
                    {
                        for (int k = 1; k < arg.length(); ++k)
                        {
                            string o(1, arg[k]);
                            if (OPT_NONE != type(o))
                                continue;
                            m_args[o] = "";
                        }
                    }
                    break;
                }
                case OPT_OPTIONAL:
                {
                    if (arg.length() > 2)
                        m_args[opt] = arg.substr(2);
                    else
                        m_args[opt] = "";
                    break;
                }
                case OPT_REQUIRED:
                {
                    if (i + 1 >= argc)
                        throw std::logic_error("option require argument: " + opt);
                    val = argv[i + 1];
                    if ("-" == val.substr(0, 1))
                        throw std::logic_error("option missing argument: " + opt);
                    m_args[opt] = val;
                    i++;
                    break;
                }
                default:
                    break;
            }
        } else
            continue;
    }
}

void Option::show() const
{
    for (const auto & pair: m_args)
    {
        std::cout << pair.first << ":" << pair.second << std::endl;
    }
}

bool Option::has(const string & opt) const
{
    if (m_opts.find(opt) == m_opts.end())
        return false;
    return m_args.find(opt) != m_args.end();
}

bool Option::get_bool(const string & opt) const
{
    return m_args.find(opt) != m_args.end();
}

int Option::get_int(const string & opt) const
{
    auto it = m_args.find(opt);
    if (it == m_args.end())
        return -1;
    std::stringstream ss;
    ss << it->second;
    int val = 0;
    ss >> val;
    return val;
}

string Option::get_string(const string & opt) const
{
    auto it = m_args.find(opt);
    if (it == m_args.end())
        return "";
    return it->second;
}

