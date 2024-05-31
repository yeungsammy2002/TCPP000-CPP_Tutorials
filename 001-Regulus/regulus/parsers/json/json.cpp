#include "json.h"

using namespace regulus::parsers;

Json::~Json()
{
    std::cout << "destructor called" << std::endl;
    clear();
}



Json::Json() : m_type(JSON_NULL) {}
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
Json::Json(const string & value) : Json(value.c_str()) {}



Json::operator bool() const
{
    assert(is_bool() && "Json object must be JSON_BOOL");
    return m_value.m_bool;
}
Json::operator int() const
{
    assert(is_int() && "Json object must be JSON_INT");
    return m_value.m_int;
}
Json::operator double() const
{
    assert(is_double() && "Json object must be JSON_DOUBLE");
    return m_value.m_double;
}
Json::operator string() const
{
    assert(is_string() && "Json object must be JSON_STRING");
    return *m_value.m_string;
}



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

void Json::copy(const Json & other)
{
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
            if (nullptr == other.m_value.m_array)
            {
                m_value.m_array = nullptr;
                break;
            }
            m_value.m_array = new std::vector<Json>();
            auto o_begin = other.m_value.m_array->begin();
            auto o_end = other.m_value.m_array->end();
            auto begin = m_value.m_array->begin();
            for (auto o_it = o_begin, it = begin; o_it != o_end; ++o_it, ++it)
            {
                m_value.m_array->push_back(Json());
                it->copy(*o_it);
            }
        }
            break;
        case JSON_OBJECT:
        {
            if (nullptr == other.m_value.m_object)
            {
                m_value.m_object = nullptr;
                break;
            }
            auto o_begin = other.m_value.m_object->begin();
            auto o_end = other.m_value.m_object->end();
            for (auto o_it = o_begin; o_it != o_end; ++o_it)
            {
                (*m_value.m_object)[o_it->first].copy(o_it->second);
            }
        }
        default:
            break;
    }
}

void Json::yazi_cpy(const Json & other)
{
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
            if (nullptr == other.m_value.m_array)
            {
                m_value.m_array = nullptr;
                break;
            }
            m_value.m_array = new std::vector<Json>();
            auto o_begin = other.m_value.m_array->begin();
            auto o_end = other.m_value.m_array->end();
            auto begin = m_value.m_array->begin();
            for (auto o_it = o_begin, it = begin; o_it != o_end; ++o_it, ++it)
            {
                m_value.m_array->push_back(*o_it);
            }
        }
            break;
        case JSON_OBJECT:
        {
            if (nullptr == other.m_value.m_object)
            {
                m_value.m_object = nullptr;
                break;
            }
            auto o_begin = other.m_value.m_object->begin();
            auto o_end = other.m_value.m_object->end();
            for (auto o_it = o_begin; o_it != o_end; ++o_it)
            {
                (*m_value.m_object)[o_it->first] = o_it->second;
            }
        }
        default:
            break;
    }
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
            delete m_value.m_string;
            m_value.m_string = nullptr;
        }
            break;
        case JSON_ARRAY:
        {
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
            for (auto it = m_value.m_object->begin(); it != m_value.m_object->end(); ++it)
            {
                it->second.clear();
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

Json::Json(const Json & other) : m_type(other.m_type)
{
    std::cout << "copy constructor called" << std::endl;
//    copy(other);
    yazi_cpy(other);
}
Json & Json::operator=(const Json & other)
{
    std::cout << "copy assignment operator called" << std::endl;
//    copy(other);
    yazi_cpy(other);
    return *this;
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
                if (it != m_value.m_array->begin())
                {
                    ss << ",";
                }
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

void Json::append(const Json & value)
{
    assert(is_array() && "Json object must be JSON_ARRAY");
    m_value.m_array->push_back(value);
}