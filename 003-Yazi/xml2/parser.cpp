#include <xml/parser.h>

using namespace demo::xml;

bool Parser::load(const string & filename)
{
    std::ifstream ifs(filename);
    if (ifs.fail())
    {
        return false;
    }
    std::ostringstream oss;
    oss << ifs.rdbuf();
    m_str = oss.str();
    m_idx = 0;
    return true;
}

bool Parser::load(const char * buf, int len)
{
    m_str.assign(buf, len);
    m_idx = 0;
    return true;
}

Xml Parser::parse()
{
    skip_white_space();
    if (0 == m_str.compare(m_idx, 5, "<?xml"))
    {
        if (!parse_declaration())
        {
            throw std::logic_error("parse declaration error");
        }
    }
    skip_white_space();
    while (0 == m_str.compare(m_idx, 4, "<!--"))
    {
        if (!parse_comment())
        {
            throw std::logic_error("parse comment error");
        }
        skip_white_space();
    }
    if (('<' == m_str[m_idx]) && (std::isalpha(m_str[m_idx + 1]) || '_' == m_str[m_idx + 1]))
    {
        return parse_element();
    }
    throw std::logic_error("parse element error");
}





void Parser::skip_white_space()
{
    while (
            ' ' == m_str[m_idx] ||
            '\n' == m_str[m_idx] ||
            '\t' == m_str[m_idx] ||
            '\r' == m_str[m_idx]
            )
    {
        ++m_idx;
    }
}

bool Parser::parse_declaration()
{
    if (0 != m_str.compare(m_idx, 5, "<?xml"))
    {
        return false;
    }
    m_idx += 5;
    std::size_t pos = m_str.find("?>", m_idx);
    if (std::string::npos == pos)
    {
        return false;
    }
    m_idx = (int) pos + 2;
    return true;
}

bool Parser::parse_comment()
{
    if (0 != m_str.compare(m_idx, 4, "<!--"))
    {
        return false;
    }
    m_idx += 4;
    std::size_t pos = m_str.find("-->", m_idx);
    if (std::string::npos == pos)
    {
        return false;
    }
    m_idx = (int) pos + 3;
    return true;
}

Xml Parser::parse_element()
{
    Xml elem;
    ++m_idx;
    skip_white_space();
    const string & name = parse_element_name();
    elem.name(name);
    while ('\0' != m_str[m_idx])
    {
        skip_white_space();
        if ('/' == m_str[m_idx])    // enable parse <name/>
        {
            if ('>' == m_str[m_idx + 1])
            {
                m_idx += 2;
                break;
            } else
            {
                throw std::logic_error("xml element is empty");
            }
        } else if ('>' == m_str[m_idx])
        {
            ++m_idx;
            string text = parse_element_text();
            if (!text.empty())
            {
                elem.text(text);
            }
        } else if ('<' == m_str[m_idx])
        {
            if ('/' == m_str[m_idx + 1])
            {
                string end_tag = "</" + name + ">";
                std::size_t pos = m_str.find(end_tag, m_idx);
                if (std::string::npos == pos)
                {
                    throw std::logic_error("xml element " + name + " end tag not found");
                }
                m_idx = (int) (pos + end_tag.size());
                break;
            } else if (0 == m_str.compare(m_idx, 4, "<!--"))
            {
                if (!parse_comment())
                {
                    throw std::logic_error("invalid xml comment");
                }
            } else
            {
                elem.append(parse_element());
            }
        } else
        {
            string key = parse_element_attr_key();
            skip_white_space();
            if ('=' != m_str[m_idx])
            {
                throw std::logic_error("xml attr error: " + key);
            }
            ++m_idx;
            skip_white_space();
            string val = parse_element_attr_val();
            elem.attr(key, val);
        }
    }
    return elem;
}

string Parser::parse_element_name()
{
    int pos = m_idx;
    if (std::isalpha(m_str[m_idx]) || ('_' == m_str[m_idx]))
    {
        ++m_idx;
        while (
                std::isalnum(m_str[m_idx]) ||
                '_' == m_str[m_idx] ||
                '-' == m_str[m_idx] ||
                ':' == m_str[m_idx] ||
                '.' == m_str[m_idx]
                )
        {
            ++m_idx;
        }
    }
    return m_str.substr(pos, m_idx - pos);
}

string Parser::parse_element_text()
{
    int pos = m_idx;
    while ('<' != m_str[m_idx])
    {
        ++m_idx;
    }
    return m_str.substr(pos, m_idx - pos);
}

string Parser::parse_element_attr_key()
{
    int pos = m_idx;
    if (std::isalpha(m_str[m_idx]) || ('_' == m_str[m_idx]))
    {
        ++m_idx;
        while (
                std::isalnum(m_str[m_idx]) ||
                '_' == m_str[m_idx] ||
                '-' == m_str[m_idx] ||
                ':' == m_str[m_idx]
                )
        {
            ++m_idx;
        }
    }
    return m_str.substr(pos, m_idx - pos);
}

string Parser::parse_element_attr_val()
{
    if ('"' != m_str[m_idx])
    {
        throw std::logic_error("xml element attr value should be in double quotes");
    }
    ++m_idx;
    int pos = m_idx;
    while ('"' != m_str[m_idx])
    {
        ++m_idx;
    }
    ++m_idx;
    return m_str.substr(pos, m_idx - pos - 1);
}

