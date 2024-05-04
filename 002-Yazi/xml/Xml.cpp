#include <xml/xml.h>

using namespace demo::xml;

Xml::Xml() : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr) {}

Xml::Xml(const string & name) : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr)
{
    m_name = new string(name);
}

void Xml::copy(const Xml & other)
{
    m_name = other.m_name;
    m_text = other.m_text;
    m_attrs = other.m_attrs;
    m_child = other.m_child;
}

Xml::Xml(const Xml & other)
{
    copy(other);
}

Xml::iterator Xml::begin()
{
    return m_child->begin();
}

Xml::iterator Xml::end()
{
    return m_child->end();
}

Xml::iterator Xml::erase(iterator it)
{
    return m_child->erase(it);
}

Xml::iterator Xml::begin() const
{
    return m_child->begin();
}

Xml::iterator Xml::end() const
{
    return m_child->end();
}

void Xml::clear()
{
    if (nullptr != m_name)
    {
        delete m_name;
        m_name = nullptr;
    }
    if (nullptr != m_text)
    {
        delete m_text;
        m_text = nullptr;
    }
    if (nullptr != m_attrs)
    {
        delete m_attrs;
        m_attrs = nullptr;
    }
    if (nullptr != m_child)
    {
        for (auto it = begin(); it != end(); ++it)
        {
            it->clear();
        }
        delete m_child;
        m_child = nullptr;
    }
}

Xml & Xml::operator=(const Xml & other)
{
    if (&other == this)
    {
        return *this;
    }
    clear();
    copy(other);
    return *this;
}

string Xml::name() const
{
    if (nullptr == m_name)
    {
        return "";
    }
    return *m_name;
}

void Xml::name(const string & name)
{
    if (nullptr != m_name)
    {
        delete m_name;
        m_name = nullptr;
    }
    m_name = new string(name);
}

string Xml::text() const
{
    if (nullptr == m_text)
    {
        return "";
    }
    return *m_text;
}

void Xml::text(const string & text)
{
    if (nullptr != m_text)
    {
        delete m_text;
        m_text = nullptr;
    }
    m_text = new string(text);
}

Value & Xml::attr(const string & key)
{
    if (nullptr == m_attrs)
    {
        m_attrs = new std::map<string, Value>();
    }
    return (*m_attrs)[key];
}

void Xml::attr(const string & key, const Value & value)
{
    if (nullptr == m_attrs)
    {
        m_attrs = new std::map<string, Value>();
    }
    (*m_attrs)[key] = value;
}

string Xml::str() const
{
    if (nullptr == m_name)
    {
        throw std::logic_error("element name is empty");
    }
    std::stringstream ss;
    ss << "<" << *m_name;
    if (nullptr != m_attrs)
    {
        for (auto it = m_attrs->begin(); it != m_attrs->end(); ++it)
        {
            ss << " " << it->first << "=\"" << string(it->second) << "\"";
        }
    }
    ss << ">";
    if (nullptr != m_child)
    {
        for (auto it = begin(); it != end(); ++it)
        {
            ss << it->str();
        }
    }
    if (nullptr != m_text)
    {
        ss << *m_text;
    }
    ss << "</" << *m_name << ">";
    return ss.str();
}

void Xml::show() const
{
    std::cout << str() << std::endl;
}

void Xml::append(const Xml & child)
{
    if (nullptr == m_child)
    {
        m_child = new std::list<Xml>();
    }
    m_child->push_back(child);
}

int Xml::size() const
{
    return m_child->size();
}

Xml & Xml::operator[](const int index)
{
    if (nullptr == m_child)
    {
        m_child = new std::list<Xml>();
    }
    int size = this->size();
    if (index < size)
    {
        auto it = begin();
        for (int i = 0; i < index; ++i)
        {
            ++it;
        }
        return *it;
    }
    if (index >= size)
    {
        for (int i = size; i <= index; ++i)
        {
            m_child->push_back(Xml());
        }
    }
    return m_child->back();
}

Xml & Xml::operator[](const char * name)
{
    return (*this)[string(name)];
}

Xml & Xml::operator[](const string & name)
{
    if (nullptr == m_child)
    {
        m_child = new std::list<Xml>();
    }
    for (auto it = begin(); it != end(); ++it)
    {
        if (name == it->name())
        {
            return *it;
        }
    }
    m_child->push_back(Xml(name));
    return m_child->back();
}

void Xml::remove(const int index)
{
    if (nullptr == m_child || index < 0 || index >= size())
    {
        return;
    }
    auto it = begin();
    for (int i = 0; i < index; ++i)
    {
        ++it;
    }
    it->clear();
    erase(it);
}

void Xml::remove(const char * name)
{
    remove(string(name));
}

void Xml::remove(const string & name)
{
    if (nullptr == m_child)
    {
        return;
    }
    for (auto it = begin(); it != end();)
    {
        if (name == it->name())
        {
            it->clear();
            it = erase(it);
        } else
        {
            ++it;
        }
    }
}