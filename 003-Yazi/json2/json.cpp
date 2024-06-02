#include <json/json.h>
#include <json/parser.h>

using namespace demo::json;

Json::Json() : m_type(JSON_NULL) {}

Json::Json(Type type) : m_type(type)
{
    switch (type)
    {
    case JSON_NULL:
        break;
    case JSON_BOOL:
    {
        m_value.m_bool = false;
    }
    break;
    case JSON_INT:
    {
        m_value.m_int = 0;
    }
    break;
    case JSON_DOUBLE:
    {
        m_value.m_double = 0.0;
    }
    break;
    case JSON_STRING:
    {
        m_value.m_string = new string("");
    }
    break;
    case JSON_ARRAY:
    {
        m_value.m_array = new std::vector<Json>();
    }
    break;
    case JSON_OBJECT:
    {
        m_value.m_object = new std::map<string, Json>();
    }
    break;
    default:
        break;
    }
}

Json::Json(bool value) : m_type(JSON_BOOL)
{
    m_value.m_bool = value;
}

Json::Json(int value) : m_type(JSON_INT)
{
    m_value.m_int = value;
}

Json::Json(double value) : m_type(JSON_DOUBLE)
{
    m_value.m_double = value;
}

Json::Json(const char* value) : m_type(JSON_STRING)
{
    m_value.m_string = new string(value);
}

Json::Json(const string& value) : m_type(JSON_STRING)
{
    m_value.m_string = new string(value);
}

Json::Json(const Json& other) : m_type(JSON_NULL)
{
    copy(other);
}

Json::Json(Json&& other)
{
    swap(other);
}

Json::~Json()
{
    clear();
}

Json::Type Json::type() const
{
    return m_type;
}

bool Json::is_null() const
{
    return JSON_NULL == m_type;
}

bool Json::is_bool() const
{
    return JSON_BOOL == m_type;
}

bool Json::is_int() const
{
    return JSON_INT == m_type;
}

bool Json::is_double() const
{
    return JSON_DOUBLE == m_type;
}

bool Json::is_string() const
{
    return JSON_STRING == m_type;
}

bool Json::is_array() const
{
    return JSON_ARRAY == m_type;
}

bool Json::is_object() const
{
    return JSON_OBJECT == m_type;
}

bool Json::as_bool() const
{
    if (JSON_BOOL != m_type)
    {
        throw std::logic_error("type error: not a bool type");
    }
    return m_value.m_bool;
}

int Json::as_int() const
{
    if (JSON_INT != m_type)
    {
        throw std::logic_error("type error: not an int type");
    }
    return m_value.m_int;
}

double Json::as_double() const
{
    if (JSON_DOUBLE != m_type)
    {
        throw std::logic_error("type error: not a double type");
    }
    return m_value.m_double;
}

string Json::as_string() const
{
    if (JSON_STRING != m_type)
    {
        throw std::logic_error("type error: not a string type");
    }
    return *m_value.m_string;
}

void Json::clear()
{
    switch (m_type)
    {
    case JSON_NULL:
    case JSON_BOOL:
    case JSON_INT:
    case JSON_DOUBLE:
        break;
    case JSON_STRING:
    {
        if (nullptr != m_value.m_string)
        {
            delete m_value.m_string;
            m_value.m_string = nullptr;
        }
    }
    break;
    case JSON_ARRAY:
    {
        if (nullptr != m_value.m_array)
        {
            for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); ++it)
            {
                it->clear();
            }
            delete m_value.m_array;
            m_value.m_array = nullptr;
        }
    }
    break;
    case JSON_OBJECT:
    {
        if (nullptr != m_value.m_object)
        {
            for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it)
            {
                (it->second).clear();
            }
            delete m_value.m_object;
            m_value.m_object = nullptr;
        }
    }
    break;
    default:
        break;
    }
    m_type = JSON_NULL;
}

Json& Json::operator=(bool value)
{
    clear();
    m_type = JSON_BOOL;
    m_value.m_bool = value;
    return *this;
}

Json& Json::operator=(int value)
{
    clear();
    m_type = JSON_INT;
    m_value.m_int = value;
    return *this;
}

Json& Json::operator=(double value)
{
    clear();
    m_type = JSON_DOUBLE;
    m_value.m_double = value;
    return *this;
}

Json& Json::operator=(const char* value)
{
    clear();
    m_type = JSON_STRING;
    m_value.m_string = new string(value);
    return *this;
}

Json& Json::operator=(const string& value)
{
    clear();
    m_type = JSON_STRING;
    m_value.m_string = new string(value);
    return *this;
}

Json& Json::operator=(const Json& other)
{
    clear();
    copy(other);
    return *this;
}

Json& Json::operator=(Json&& other) noexcept
{
    swap(other);
    return *this;
}

Json::operator bool()
{
    return as_bool();
}

Json::operator bool() const
{
    return as_bool();
}

Json::operator int()
{
    return as_int();
}

Json::operator int() const
{
    return as_int();
}

Json::operator double()
{
    return as_double();
}

Json::operator double() const
{
    return as_double();
}

Json::operator string()
{
    return as_string();
}

Json::operator string() const
{
    return as_string();
}

string Json::str() const
{
    std::stringstream ss;
    switch (m_type)
    {
    case JSON_NULL:
    {
        ss << "null";
    }
    break;
    case JSON_BOOL:
    {
        ss << (m_value.m_bool ? "true" : "false");
    }
    break;
    case JSON_INT:
    {
        ss << m_value.m_int;
    }
    break;
    case JSON_DOUBLE:
    {
        ss << m_value.m_double;
    }
    break;
    case JSON_STRING:
    {
        ss << "\"" << *(m_value.m_string) << "\"";
    }
    break;
    case JSON_ARRAY:
    {
        ss << "[";
        for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); ++it)
        {
            ss << (m_value.m_array->begin() != it ? "," : "");
            ss << it->str();
        }
        ss << "]";
    }
    break;
    case JSON_OBJECT:
    {
        ss << "{";
        for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it)
        {
            ss << (m_value.m_object->begin() != it ? "," : "");
            ss << "\"" << it->first << "\"" << ":" << (it->second).str();
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

void Json::append(const Json& value)
{
    if (!is_array())
    {
        clear();
        m_type = JSON_ARRAY;
        m_value.m_array = new std::vector<Json>();
    }
    m_value.m_array->push_back(value);
}

void Json::append(Json&& value)
{
    if (!is_array())
    {
        clear();
        m_type = JSON_ARRAY;
        m_value.m_array = new std::vector<Json>();
    }
    m_value.m_array->push_back(std::move(value));
}

bool Json::has(int index)
{
    if (!is_array())
    {
        return false;
    }
    int size = (int)m_value.m_array->size();
    return index >= 0 && index < size;
}

bool Json::has(const char* key) const
{
    if (!is_object())
    {
        return false;
    }
    return m_value.m_object->end() != m_value.m_object->find(key);
}

bool Json::has(const string& key) const
{
    return has(key.c_str());
}

Json Json::get(int index)
{
    if (!has(index))
    {
        return Json();
    }
    return m_value.m_array->at(index);
}

Json Json::get(const char* key)
{
    if (!has(key))
    {
        return Json();
    }
    return (*m_value.m_object)[key];
}

Json Json::get(const string& key)
{
    return get(key.c_str());
}

void Json::remove(int index)
{
    if (!has(index))
    {
        return;
    }
    (m_value.m_array->at(index)).clear();
    m_value.m_array->erase(m_value.m_array->begin() + index);
}

void Json::remove(const char* key)
{
    if (!is_object())
    {
        return;
    }
    auto it = m_value.m_object->find(key);
    if (m_value.m_object->end() == it)
    {
        return;
    }
    it->second.clear();
    m_value.m_object->erase(it);
}

void Json::remove(const string& key)
{
    return remove(key.c_str());
}

Json& Json::operator[](int index)
{
    if (!is_array())
    {
        throw std::logic_error("type error: not an array");
    }
    if (!has(index))
    {
        throw std::logic_error("out of range");
    }
    return m_value.m_array->at(index);
}

Json& Json::operator[](const char* key)
{
    if (!is_object())
    {
        clear();
        m_type = JSON_OBJECT;
        m_value.m_object = new std::map<string, Json>();
    }
    return (*m_value.m_object)[key];
}

Json& Json::operator[](const string& key)
{
    return (*this)[key.c_str()];
}

int Json::size() const
{
    switch (m_type)
    {
    case JSON_ARRAY:
        return (int)m_value.m_array->size();
    case JSON_OBJECT:
        return (int)m_value.m_object->size();
    default:
        break;
    }
    return -1;
}

bool Json::empty() const
{
    switch (m_type)
    {
    case JSON_NULL:
        return true;
    case JSON_ARRAY:
        return m_value.m_array->empty();
    case JSON_OBJECT:
        return m_value.m_object->empty();
    default:
        break;
    }
    return false;
}

void Json::copy(const Json& other)
{
    clear();
    m_type = other.m_type;
    switch (m_type)
    {
    case JSON_NULL:
        break;
    case JSON_BOOL:
    case JSON_INT:
    case JSON_DOUBLE:
    {
        m_value = other.m_value;
    }
    break;
    case JSON_STRING:
    {
        if (nullptr != other.m_value.m_string)
        {
            m_value.m_string = new string(*other.m_value.m_string);
        }
    }
    break;
    case JSON_ARRAY:
    {
        if (nullptr != other.m_value.m_array)
        {
            m_value.m_array = new std::vector<Json>();
            for (auto it = other.m_value.m_array->begin(); it != other.m_value.m_array->end(); ++it)
            {
                m_value.m_array->push_back(*it);
            }
        }
    }
    break;
    case JSON_OBJECT:
    {
        if (nullptr != other.m_value.m_object)
        {
            m_value.m_object = new std::map<string, Json>();
            for (auto it = other.m_value.m_object->begin(); it != other.m_value.m_object->end(); ++it)
            {
                (*m_value.m_object)[it->first] = it->second;
            }
        }
    }
    break;
    default:
        break;
    }
}

void Json::swap(Json& other)
{
    Type type = m_type;
    Value value = m_value;
    m_type = other.m_type;
    m_value = other.m_value;
    other.m_type = type;
    other.m_value = value;
}

void Json::parse(const string& filename)
{
    clear();
    Parser p;
    p.load(filename);
    *this = p.parse();
}

void Json::parse(const char* buff, int len)
{
    clear();
    Parser p;
    p.load(buff, len);
    *this = p.parse();
}

