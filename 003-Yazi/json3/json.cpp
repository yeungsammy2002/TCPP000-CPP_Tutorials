#include <json/json.h>

using namespace demo::json;





Json::Json() : m_type(JSON_NULL) {}

Json::Json(Type type) : m_type(type)
{
    switch (m_type)
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

Json::Json(const char * value) : m_type(JSON_STRING)
{
    m_value.m_string = new string(value);
}

Json::Json(const string & value) : m_type(JSON_STRING)
{
    m_value.m_string = new string(value);
}

Json::Json(const Json & other) : m_type(other.m_type)
{
    copy(other);
}

Json::Json(Json && other) : m_type(other.m_type)
{
    move(std::move(other));
}





Json::~Json()
{
    clear();
}





Json & Json::operator=(const Json & other)
{
    copy(other);
    return *this;
}

Json & Json::operator=(Json && other) noexcept
{
    if (&other == this)
    {
        return *this;
    }
    clear();
    move(std::move(other));
    return *this;
}





bool Json::as_bool() const
{
    assert(is_bool() && "type must be JSON_BOOL");
    return m_value.m_bool;
}

int Json::as_int() const
{
    assert(is_int() && "type must be JSON_INT");
    return m_value.m_int;
}

double Json::as_double() const
{
    assert(is_double() && "type must be JSON_DOUBLE");
    return m_value.m_double;
}

string Json::as_string() const
{
    assert(is_string() && "type must be JSON_STRING");
    return *m_value.m_string;
}





void Json::append(const Json & value)
{
    assert(is_array() && "type must be JSON_ARRAY");
    if (nullptr == m_value.m_array)
    {
        m_value.m_array = new std::vector<Json>();
    }
    m_value.m_array->push_back(value);
}

void Json::append(Json && value)
{
    assert(is_array() && "type must be JSON_ARRAY");
    if (nullptr == m_value.m_array)
    {
        m_value.m_array = new std::vector<Json>();
    }
    m_value.m_array->push_back(std::move(value));
}





bool Json::has(const int index) const
{
    assert(is_array() && "type must be JSON_ARRAY");
    const auto size = m_value.m_array->size();
    if (size == 0 || nullptr == m_value.m_array)
    {
        return false;
    }
    return (index >= 0) && (index < size);
}

bool Json::has(const char * key) const
{
    assert(is_object() && "type must be JSON_OBJECT");
    if (nullptr == m_value.m_object)
    {
        return false;
    }
    return m_value.m_object->find(key) != m_value.m_object->end();
}

bool Json::has(const string & key) const
{
    return this->has(key.c_str());
}





Json Json::get(const int index) const
{
    assert(has(index) && "index must be in range");
    return m_value.m_array->at(index);
}

Json Json::get(const char * key) const
{
    assert(has(key) && "object must has the key");
    return (m_value.m_object->find(key))->second;
}

Json Json::get(const string & key) const
{
    return this->get(key.c_str());
}





Json & Json::operator[](const int index)
{
    assert(has(index) && "array must has the index");
    return m_value.m_array->at(index);
}

Json & Json::operator[](const char * key)
{
    assert(is_object() && "type must be JSON_OBJECT");
    if (nullptr == m_value.m_object)
    {
        m_value.m_object = new std::map<string, Json>();
    }
    return (*m_value.m_object)[key];
}

Json & Json::operator[](const string & key)
{
    return (*this)[key.c_str()];
}





void Json::remove(const int index)
{
    if (!has(index))
    {
        return;
    }
    auto it = m_value.m_array->begin() + index;
    it->clear();
    m_value.m_array->erase(it);
}

void Json::remove(const char * key)
{
    if (!has(key))
    {
        return;
    }
    auto it = m_value.m_object->find(key);
    it->second.clear();
    m_value.m_object->erase(it);
}

void Json::remove(const string & key)
{
    return remove(key.c_str());
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
            ss << "\"" << *m_value.m_string << "\"";
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
                ss << "\"" << it->first << "\":" << it->second.str();
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





int Json::size() const
{
    switch (m_type)
    {
        case JSON_ARRAY:
        {
            if (nullptr == m_value.m_array)
            {
                return 0;
            }
            return (int) m_value.m_array->size();
        }
        case JSON_OBJECT:
        {
            if (nullptr == m_value.m_object)
            {
                return 0;
            }
            return (int) m_value.m_object->size();
        }
        default:
            break;
    }
    throw std::logic_error("type must be JSON_ARRAY or JSON_OBJECT");
}

bool Json::empty() const
{
    switch (m_type)
    {
        case JSON_NULL:
            return true;
        case JSON_ARRAY:
        {
            return (nullptr == m_value.m_array) || (m_value.m_array->empty());
        }
        case JSON_OBJECT:
        {
            return (nullptr == m_value.m_object) || (m_value.m_object->empty());
        }
        default:
            break;
    }
    throw std::logic_error("type must be JSON_NULL, JSON_ARRAY or JSON_OBJECT");
}

Json::Type Json::type() const
{
    return m_type;
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
            if (nullptr == m_value.m_string)
            {
                break;
            }
            delete m_value.m_string;
            m_value.m_string = nullptr;
        }
            break;
        case JSON_ARRAY:
        {
            if (nullptr == m_value.m_array)
            {
                break;
            }
            for (auto it = m_value.m_array->begin(); it != m_value.m_array->end(); ++it)
            {
                it->clear();
            }
            delete m_value.m_array;
            m_value.m_array = nullptr;
        }
            break;
        case JSON_OBJECT:
        {
            if (nullptr == m_value.m_object)
            {
                break;
            }
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
    m_type = JSON_NULL;
}





void Json::copy(const Json & other)
{
    if (&other == this)
    {
        return;
    }
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
            m_value.m_string = new string(*other.m_value.m_string);
        }
            break;
        case JSON_ARRAY:
        {
            m_value.m_array = new std::vector<Json>();
            auto o_begin = other.m_value.m_array->begin();
            auto o_end = other.m_value.m_array->end();
            for (auto o_it = o_begin; o_it != o_end; ++o_it)
            {
                m_value.m_array->push_back(*o_it);
            }
        }
            break;
        case JSON_OBJECT:
        {
            m_value.m_object = new std::map<string, Json>();
            auto o_begin = other.m_value.m_object->begin();
            auto o_end = other.m_value.m_object->end();
            for (auto o_it = o_begin; o_it != o_end; ++o_it)
            {
                (*m_value.m_object)[o_it->first] = o_it->second;
            }
        }
            break;
        default:
            break;
    }
}

void Json::swap(Json & other)
{
    Type type = m_type;
    Value value = m_value;
    m_type = other.m_type;
    m_value = other.m_value;
    other.m_type = type;
    other.m_value = value;
}

void Json::move(Json && other)
{
    if (&other == this)
    {
        return;
    }
    clear();
    swap(other);
}

