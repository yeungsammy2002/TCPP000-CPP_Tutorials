#include <json/json.h>

using namespace demo::json;

Json::Json() : m_type(json_null) {}

void Json::copy(const Json & other)
{
    m_type = other.m_type;
    switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
        {
            m_value.m_bool = other.m_value.m_bool;
        }
            break;
        case json_int:
        {
            m_value.m_int = other.m_value.m_int;
        }
            break;
        case json_double:
        {
            m_value.m_double = other.m_value.m_double;
        }
            break;
        case json_string:
        {
            m_value.m_string = other.m_value.m_string;
        }
            break;
        case json_array:
        {
            m_value.m_array = other.m_value.m_array;
        }
            break;
        case json_object:
        {
            m_value.m_object = other.m_value.m_object;
        }
            break;
        default:
            break;
    }
}

Json::Json(const Json & other)
{
    copy(other);
}

Json::iterator Json::begin()
{
    return m_value.m_array->begin();
}

Json::iterator Json::end()
{
    return m_value.m_array->end();
}

Json::iterator Json::erase(iterator it)
{
    return m_value.m_array->erase(it);
}

Json::iterator Json::begin() const
{
    return m_value.m_array->begin();
}

Json::iterator Json::end() const
{
    return m_value.m_array->end();
}

void Json::clear()
{
    switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
        {
            m_value.m_bool = false;
        }
            break;
        case json_int:
        {
            m_value.m_int = 0;
        }
            break;
        case json_double:
        {
            m_value.m_double = 0.0;
        }
            break;
        case json_string:
        {
            delete m_value.m_string;
            m_value.m_string = nullptr;
        }
            break;
        case json_array:
        {
            for (auto it = begin(); it != end(); ++it)
            {
                it->clear();
            }
            delete m_value.m_array;
            m_value.m_array = nullptr;
        }
            break;
        case json_object:
        {
            for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it)
            {
                (it->second).clear();
            }
            delete m_value.m_object;
            m_value.m_object = nullptr;
        }
            break;
        default:
            break;
    }
    m_type = json_null;
}

Json & Json::operator=(const Json & other)
{
    clear();
    copy(other);
    return *this;
}

bool Json::operator==(const Json & other) const
{
    if (other.m_type != m_type)
    {
        return false;
    }
    switch (m_type)
    {
        case json_null:
        {
            return true;
        }
        case json_bool:
        {
            return other.m_value.m_bool == m_value.m_bool;
        }
        case json_int:
        {
            return other.m_value.m_int == m_value.m_int;
        }
        case json_double:
        {
            return other.m_value.m_double == m_value.m_double;
        }
        case json_string:
        {
            return *(other.m_value.m_string) == *(m_value.m_string);
        }
        case json_array:
        {
            return other.m_value.m_array == m_value.m_array;
        }
        case json_object:
        {
            return other.m_value.m_object == m_value.m_object;
        }
        default:
            break;
    }
    return false;
}

bool Json::operator!=(const Json & other) const
{
    return !(other == *this);
}

Json::Json(bool value) : m_type(json_bool)
{
    m_value.m_bool = value;
}

Json::Json(int value) : m_type(json_int)
{
    m_value.m_int = value;
}

Json::Json(double value) : m_type(json_double)
{
    m_value.m_double = value;
}

Json::Json(const char * value) : m_type(json_string)
{
    m_value.m_string = new string(value);
}

Json::Json(const string & value) : m_type(json_string)
{
    m_value.m_string = new string(value);
}

Json::Json(Type type)
{
    m_type = type;
    switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
        {
            m_value.m_bool = false;
        }
            break;
        case json_int:
        {
            m_value.m_int = 0;
        }
            break;
        case json_double:
        {
            m_value.m_double = 0.0;
        }
            break;
        case json_string:
        {
            m_value.m_string = new string();
        }
            break;
        case json_array:
        {
            m_value.m_array = new std::vector<Json>();
        }
            break;
        case json_object:
        {
            m_value.m_object = new std::map<string, Json>();
        }
            break;
        default:
            break;
    }
}

Json::operator bool()
{
    if (!is_bool())
    {
        throw std::logic_error("type error: not a bool value");
    }
    return m_value.m_bool;
}

Json::operator int()
{
    if (!is_int())
    {
        throw std::logic_error("type error: not a int value");
    }
    return m_value.m_int;
}

Json::operator double()
{
    if (!is_double())
    {
        throw std::logic_error("type error: not a double value");
    }
    return m_value.m_double;
}

Json::operator string()
{
    if (!is_string())
    {
        throw std::logic_error("type error: not a string value");
    }
    return *(m_value.m_string);
}

bool Json::as_bool() const
{
    if (!is_bool())
    {
        throw std::logic_error("type error: not a bool value");
    }
    return m_value.m_bool;
}

int Json::as_int() const
{
    if (!is_int())
    {
        throw std::logic_error("type error: not a int value");
    }
    return m_value.m_int;
}

double Json::as_double() const
{
    if (!is_double())
    {
        throw std::logic_error("type error: not a double value");
    }
    return m_value.m_double;
}

string Json::as_string() const
{
    if (!is_string())
    {
        throw std::logic_error("type error: not a string value");
    }
    return *(m_value.m_string);
}

void Json::append(const Json & other)
{
    if (json_array != m_type)
    {
        clear();
        m_type = json_array;
        m_value.m_array = new std::vector<Json>();
    }
    m_value.m_array->push_back(other);
}

int Json::size() const
{
    return m_value.m_array->size();
}

Json & Json::operator[](const int index)
{
    if (json_array != m_type)
    {
        clear();
        m_type = json_array;
        m_value.m_array = new std::vector<Json>();
    }
    int size = this->size();
    if (index >= size)
    {
        for (int i = size; i <= index; ++i)
        {
            m_value.m_array->push_back(Json());
        }
    }
    return m_value.m_array->at(index);
}

string Json::str() const
{
    std::stringstream ss;
    switch (m_type)
    {
        case json_null:
        {
            ss << "null";
        }
            break;
        case json_bool:
        {
            ss << (m_value.m_bool ? "true" : "false");
        }
            break;
        case json_int:
        {
            ss << m_value.m_int;
        }
            break;
        case json_double:
        {
            ss << m_value.m_double;
        }
            break;
        case json_string:
        {
            ss << "\"" << *(m_value.m_string) << "\"";
        }
            break;
        case json_array:
        {
            ss << "[";
            for (auto it = begin(); it != end(); ++it)
            {
                if (it != begin())
                {
                    ss << ",";
                }
                ss << it->str();
            }
            ss << "]";
        }
            break;
        case json_object:
        {
            ss << "{";
            for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it)
            {
                if (it != m_value.m_object->begin())
                {
                    ss << ",";
                }
                ss << "\"" << it->first << "\":" << (it->second).str();
            }
            ss << "}";
        }
            break;
        default:
            break;
    }
    return ss.str();
}

void Json::show() const
{
    std::cout << str() << std::endl;
}

Json & Json::operator[](const char * key)
{
    return (*this)[string(key)];
}

Json & Json::operator[](const string & key)
{
    if (json_object != m_type)
    {
        clear();
        m_type = json_object;
        m_value.m_object = new std::map<string, Json>();
    }
    return (*m_value.m_object)[key];
}

bool Json::has(const int index) const
{
    return ((is_array()) && (index >= 0) && (index < size()));
}

bool Json::has(const char * key) const
{
    return has(string(key));
}

bool Json::has(const string & key) const
{
    if (!is_object())
    {
        return false;
    }
    return m_value.m_object->find(key) != m_value.m_object->end();
}

void Json::remove(const int index)
{
    if (!has(index))
    {
        return;
    }
    erase(begin() + index);
}

void Json::remove(const char * key)
{
    remove(string(key));
}

void Json::remove(const string & key)
{
    if (!has(key))
    {
        return;
    }
    m_value.m_object->erase(key);
}
