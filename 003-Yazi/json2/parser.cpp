#include <json/parser.h>

using namespace demo::json;

void Parser::load(const string & filename)
{
    std::ifstream ifs(filename);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    m_str = oss.str();
    m_idx = 0;
}

void Parser::load(const char * buff, int len)
{
    m_str.assign(buff, len);
    m_idx = 0;
}

void Parser::skip_white_space()
{
    while (' ' == m_str[m_idx]
           || '\r' == m_str[m_idx]
           || '\n' == m_str[m_idx]
           || '\t' == m_str[m_idx]
            )
    {
        m_idx++;
    }
}

char Parser::get_next_token()
{
    skip_white_space();
    if (m_str.size() == m_idx)
    {
        throw std::logic_error("unexpected input");
    }
    return m_str[m_idx++];
}

bool Parser::in_range(int x, int lower, int upper) const
{
    return (x >= lower) && (x <= upper);
}

Json Parser::parse()
{
    char ch = get_next_token();
    switch (ch)
    {
        case 'n':
        {
            m_idx--;
            return parse_null();
        }
        case 't':
        case 'f':
        {
            m_idx--;
            return parse_bool();
        }
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            m_idx--;
            return parse_number();
        }
        case '"':
        {
            return Json(parse_string());
        }
        case '[':
        {
            return parse_array();
        }
        case '{':
        {
            return parse_object();
        }
        default:
            break;
    }
    throw std::logic_error("unexpected character");
}

Json Parser::parse_null()
{
    if (0 == m_str.compare(m_idx, 4, "null"))
    {
        m_idx += 4;
        return Json();
    }
    throw std::logic_error("parse null error");
}

Json Parser::parse_bool()
{
    if (0 == m_str.compare(m_idx, 4, "true"))
    {
        m_idx += 4;
        return Json(true);
    }
    if (0 == m_str.compare(m_idx, 5, "false"))
    {
        m_idx += 5;
        return Json(false);
    }
    throw std::logic_error("parse bool error");
}

Json Parser::parse_number()
{
    int pos = m_idx;
    if ('-' == m_str[m_idx])
    {
        m_idx++;
    }
    if ('0' == m_str[m_idx])
    {
        m_idx++;
    } else if (in_range(m_str[m_idx], '1', '9'))
    {
        m_idx++;
        while (in_range(m_str[m_idx], '0', '9'))
        {
            m_idx++;
        }
    } else
    {
        throw std::logic_error("invalid character");
    }
    if ('.' != m_str[m_idx])
    {
        return Json(std::atoi(m_str.c_str() + pos));
    }
    m_idx++;
    if (!in_range(m_str[m_idx], '0', '9'))
    {
        throw std::logic_error("invalid float number");
    }
    while (in_range(m_str[m_idx], '0', '9'))
    {
        m_idx++;
    }
    return Json(std::atof(m_str.c_str() + pos));
}

string Parser::parse_string()
{
    int pos = m_idx;
    while (true)
    {
        if (m_idx == m_str.size())
        {
            throw std::logic_error("unexpected end of input");
        }
        char ch = get_next_token();
        if ('"' == ch)
        {
            break;
        }
        if ('\\' == ch)
        {
            ch = get_next_token();
            switch (ch)
            {
                case 'b':
                case 't':
                case 'n':
                case 'f':
                case 'r':           // not working properly
                case '"':
                case '\\':
                    break;
                case 'u':
                {
                    m_idx += 4;
                }
                    break;
                default:
                    break;
            }
        }
    }
    return m_str.substr(pos, m_idx - pos - 1);
}

Json Parser::parse_array()
{
    Json arr(Json::JSON_ARRAY);
    char ch = get_next_token();
    if (']' == ch)
    {
        return arr;
    }
    m_idx--;
    while (true)
    {
        arr.append(parse());
        ch = get_next_token();
        if (']' == ch)
        {
            break;
        }
        if (',' != ch)
        {
            throw std::logic_error("expected ',' in array");
        }
    }
    return arr;
}

Json Parser::parse_object()
{
    Json obj(Json::JSON_OBJECT);
    char ch = get_next_token();
    if ('}' == ch)
    {
        return obj;
    }
    m_idx--;
    while (true)
    {
        ch = get_next_token();
        if ('"' != ch)
        {
            throw std::logic_error("invalid object key");
        }
        string key = parse_string();
        ch = get_next_token();
        if (':' != ch)
        {
            throw std::logic_error("expected ':' in object");
        }
        obj[key] = parse();
        ch = get_next_token();
        if ('}' == ch)
        {
            break;
        }
        if (ch != ',')
        {
            throw std::logic_error("expected ',' in object");
        }
    }
    return obj;
}

