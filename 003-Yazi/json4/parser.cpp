#include <json/parser.h>





void Parser::load(const string & filename)
{
    std::ifstream ifs(filename);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    m_str = oss.str();
    m_idx = 0;
}

void Parser::load(const char * buf, int len)
{
    m_str.assign(buf, len);
    m_idx = 0;
}





Json Parser::parse()
{
    char ch = get_next_token();
    switch (ch)
    {
        case 'n':
        {
            --m_idx;
            return parse_null();
        }
        case 't':
        case 'f':
        {
            --m_idx;
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
            --m_idx;
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





void Parser::skip_white_space()
{
    while (
            m_str[m_idx] == ' ' ||
            m_str[m_idx] == '\n' ||
            m_str[m_idx] == '\r' ||
            m_str[m_idx] == '\b'
            )
    {
        ++m_idx;
    }
}

char Parser::get_next_token()
{
    skip_white_space();
    if (m_idx >= m_str.size())
    {
        throw std::logic_error("unexpected character");
    }
    return m_str[m_idx++];
}





Json Parser::parse_null()
{
    if (m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return Json();
    }
    throw std::logic_error("parse null error");
}

Json Parser::parse_bool()
{
    if (m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return Json(true);
    } else if (m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return Json(false);
    }
    throw std::logic_error("parse bool error");
}

Json Parser::parse_number()
{
    int pos = m_idx;

    if (m_str[m_idx] == '-')
    {
        ++m_idx;
    }

    if (m_str[m_idx] == '0')
    {
        ++m_idx;
    } else if (in_range(m_str[m_idx], '1', '9'))
    {
        ++m_idx;
        while (in_range(m_str[m_idx], '0', '9'))
        {
            ++m_idx;
        }
    } else
    {
        throw std::logic_error("parse number error");
    }

    if (m_str[m_idx] != '.')
    {
        return Json(std::atoi(m_str.c_str() + pos));
    }

    ++m_idx;
    if (!in_range(m_str[m_idx], '0', '9'))
    {
        throw std::logic_error("parse float number error");
    }
    while (in_range(m_str[m_idx], '1', '9'))
    {
        ++m_idx;
    }

    return Json(std::atof(m_str.c_str() + pos));
}

string Parser::parse_string()
{
    int pos = m_idx;
    while (true)
    {
        char ch = get_next_token();
        if (ch == '"')
        {
            break;
        }

        if (ch == '\\')
        {
            ch = get_next_token();
            switch (ch)
            {
                case 'f':
                case 't':
                case 'b':
                case 'n':
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
    if (ch == ']')
    {
        return arr;
    } else
    {
        --m_idx;
    }

    while (true)
    {
        arr.append(parse());
        ch = get_next_token();
        if (ch == ']')
        {
            break;
        } else if (ch != ',')
        {
            throw std::logic_error("parse array error: expect ',' in array");
        }
    }

    return arr;
}

Json Parser::parse_object()
{
    Json obj(Json::JSON_OBJECT);
    char ch = get_next_token();
    if (ch == '{')
    {
        return obj;
    } else
    {
        --m_idx;
    }

    while (true)
    {
        ch = get_next_token();
        if (ch != '"')
        {
            throw std::logic_error("parse object error: invalid object key");
        }
        string key = parse_string();
        ch = get_next_token();
        if (ch != ':')
        {
            throw std::logic_error("parse object error: expect ':' in object");
        }
        obj[key] = parse();
        ch = get_next_token();
        if (ch == '}')
        {
            break;
        } else if (ch != ',')
        {
            throw std::logic_error("parse object error: expect ',' in object");
        }
    }

    return obj;
}

