#include <json/json.h>
#include <json/parser.h>





Json::~Json()
{
    clear();
}





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
            m_value.m_string = new string();
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

Json::Json(const Json & other)
{
    copy(other);
}

Json::Json(Json && other)
{
    swap(other);
}





string Json::str() const
{
    std::ostringstream oss;
    switch (m_type)
    {
        case JSON_NULL:
        {
            oss << "null";
        }
            break;
        case JSON_BOOL:
        {
            oss << (m_value.m_bool ? "true" : "false");
        }
            break;
        case JSON_INT:
        {
            oss << m_value.m_int;
        }
            break;
        case JSON_DOUBLE:
        {
            oss << m_value.m_double;
        }
            break;
        case JSON_STRING:
        {
            oss << '\"';
            if (!is_str_null())
            {
                oss << *(m_value.m_string);
            }
            oss << '\"';
        }
            break;
        case JSON_ARRAY:
        {
            oss << '[';
            if (!is_arr_null())
            {
                auto begin = m_value.m_array->begin();
                auto end = m_value.m_array->end();
                for (auto it = begin; it != end; ++it)
                {
                    if (it != begin)
                    {
                        oss << ",";
                    }
                    oss << it->str();
                }
            }
            oss << ']';
        }
            break;
        case JSON_OBJECT:
        {
            oss << '{';
            if (!is_obj_null())
            {
                auto begin = m_value.m_object->begin();
                auto end = m_value.m_object->end();
                for (auto it = begin; it != end; ++it)
                {
                    if (it != begin)
                    {
                        oss << ",";
                    }
                    oss << "\"" << it->first << "\":" << (it->second).str();
                }
            }
            oss << '}';
        }
            break;
        default:
            break;
    }
    return oss.str();
}

std::ostream & operator<<(std::ostream & os, const Json & value)
{
    os << value.str();
    return os;
}





bool Json::as_bool() const
{
    assert(is_bool() && "type must be bool");
    return m_value.m_bool;
}

int Json::as_int() const
{
    assert(is_int() && "type must be int");
    return m_value.m_int;
}

double Json::as_double() const
{
    assert(is_double() && "type must be double");
    return m_value.m_double;
}

string Json::as_string() const
{
    assert(is_string() && "type must be string");
    if (is_str_null())
    {
        return "";
    }
    return *(m_value.m_string);
}





Json & Json::operator=(bool value)
{
    clear();
    m_type = JSON_BOOL;
    m_value.m_bool = value;
    return *this;
}

Json & Json::operator=(int value)
{
    clear();
    m_type = JSON_INT;
    m_value.m_int = value;
    return *this;
}

Json & Json::operator=(double value)
{
    clear();
    m_type = JSON_DOUBLE;
    m_value.m_double = value;
    return *this;
}

Json & Json::operator=(const char * value)
{
    clear();
    m_type = JSON_STRING;
    m_value.m_string = new string(value);
    return *this;
}

Json & Json::operator=(const string & value)
{
    clear();
    m_type = JSON_STRING;
    m_value.m_string = new string(value);
    return *this;
}

Json & Json::operator=(const Json & other)
{
    if (this != &other)
    {
        clear();
    }
    copy(other);
    return *this;
}

Json & Json::operator=(Json && other)
{
    swap(other);
    return *this;
}





int Json::size() const
{
    switch (m_type)
    {
        case JSON_ARRAY:
        {
            if (!is_arr_null())
            {
                return m_value.m_array->size();
            }
        }
        case JSON_OBJECT:
        {
            if (!is_obj_null())
            {
                return m_value.m_object->size();
            }
        }
        default:
            break;
    }
    return -1;
}

bool Json::empty() const
{
    switch (m_type)
    {
        case JSON_STRING:
        {
            if (is_str_null())
            {
                return true;
            }
            return m_value.m_string->empty();
        }
        case JSON_ARRAY:
        {
            if (is_arr_null())
            {
                return true;
            }
            return m_value.m_array->empty();
        }
        case JSON_OBJECT:
        {
            if (is_obj_null())
            {
                return true;
            }
            return m_value.m_object->empty();
        }
        default:
            break;
    }
    return false;
}





void Json::append(const Json & value)
{
    if (!is_array() || is_arr_null())
    {
        clear();
        m_type = JSON_ARRAY;
        m_value.m_array = new std::vector<Json>();
    }
    m_value.m_array->push_back(value);
}

void Json::append(Json && value)
{
    if (!is_array() || is_arr_null())
    {
        clear();
        m_type = JSON_ARRAY;
        m_value.m_array = new std::vector<Json>();
    }
    m_value.m_array->push_back(std::move(value));
}

bool Json::has(const int index) const
{
    if (index < 0 || !is_array() || is_arr_null())
    {
        return false;
    }

    return index < m_value.m_array->size();
}

Json Json::get(const int index) const
{
    assert(has(index) && "index must be exist in array");
    return *(begin() + index);
}

Json & Json::operator[](const int index)
{
    if (!is_array() || is_arr_null())
    {
        clear();
        m_type = JSON_ARRAY;
        m_value.m_array = new std::vector<Json>();
    }
    if (index >= size())
    {
        for (int i = size(); i <= index; ++i)
        {
            m_value.m_array->push_back(Json());
        }
    }
    return m_value.m_array->at(index);
}

void Json::remove(const int index)
{
    if (!has(index))
    {
        return;
    }
    m_value.m_array->erase(begin() + index);
}





bool Json::has(const char * key) const
{
    if (!is_object() || is_obj_null())
    {
        return false;
    }
    return m_value.m_object->find(key) != m_value.m_object->end();
}

bool Json::has(const string & key) const
{
    return has(key.c_str());
}

Json Json::get(const char * key) const
{
    assert(has(key) && "key must be exist in object");
    auto it = m_value.m_object->find(key);
    return it->second;
}

Json Json::get(const string & key) const
{
    return get(key.c_str());
}

Json & Json::operator[](const char * key)
{
    if (!is_object() || is_obj_null())
    {
        clear();
        m_type = JSON_OBJECT;
        m_value.m_object = new std::map<string, Json>();
    }
    return (*m_value.m_object)[key];
}

Json & Json::operator[](const string & key)
{
    return (*this)[key.c_str()];
}

void Json::remove(const char * key)
{
    if (!has(key))
    {
        return;
    }
    m_value.m_object->erase(key);
}

void Json::remove(const string & key)
{
    remove(key.c_str());
}





void Json::parse(const string & filename)
{
    clear();
    Parser p;
    p.load(filename);
    *this = p.parse();
}

void Json::parse(const char * buf, int len)
{
    clear();
    Parser p;
    p.load(buf, len);
    *this = p.parse();
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
            if (!is_str_null())
            {
                delete m_value.m_string;
                m_value.m_string = nullptr;
            }
        }
            break;
        case JSON_ARRAY:
        {

            if (!is_arr_null())
            {
                auto begin = m_value.m_array->begin();
                auto end = m_value.m_array->end();
                for (auto it = begin; it != end; ++it)
                {
                    it->clear();
                }
                delete m_value.m_array;
                m_value.m_array = nullptr;
            }
        }
        case JSON_OBJECT:
        {
            if (!is_obj_null())
            {
                auto begin = m_value.m_object->begin();
                auto end = m_value.m_object->end();
                for (auto it = begin; it != end; ++it)
                {
                    (it->second).clear();
                }
                delete m_value.m_object;
                m_value.m_object = nullptr;
            }
        }
        default:
            break;
    }
    m_type = JSON_NULL;
}

void Json::copy(const Json & other)
{

    m_type = other.m_type;
    switch (other.m_type)
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
            if (!other.is_str_null())
            {
                m_value.m_string = new string(*other.m_value.m_string);
            }
        }
            break;
        case JSON_ARRAY:
        {
            if (!other.is_arr_null())
            {
                m_value.m_array = new std::vector<Json>();
                auto begin = other.m_value.m_array->begin();
                auto end = other.m_value.m_array->end();
                for (auto it = begin; it != end; ++it)
                {
                    m_value.m_array->push_back(*it);
                }
            }
        }
            break;
        case JSON_OBJECT:
        {

            if (!other.is_obj_null())
            {
                m_value.m_object = new std::map<string, Json>();
                auto begin = other.m_value.m_object->begin();
                auto end = other.m_value.m_object->end();
                for (auto it = begin; it != end; ++it)
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

void Json::swap(Json & other)
{
    Type type = m_type;
    Value value = m_value;
    m_type = other.m_type;
    m_value = other.m_value;
    other.m_type = type;
    other.m_value = value;
}

