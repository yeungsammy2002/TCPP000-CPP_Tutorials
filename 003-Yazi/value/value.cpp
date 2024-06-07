#include <utilities/value.h>

using namespace demo::utilities;





Value::Value() : m_type(V_NULL), m_value("") {}

Value::Value(bool value) : m_type(V_BOOL)
{
    m_value = value ? "true" : "false";
}

Value::Value(int value) : m_type(V_INT)
{
    m_value = std::to_string(value);
}

Value::Value(float value) : m_type(V_FLOAT)
{
    m_value = std::to_string(value);
}

Value::Value(double value) : m_type(V_DOUBLE)
{
    m_value = std::to_string(value);
}

Value::Value(const char * value) : m_type(V_STRING)
{
    m_value = value;
}

Value::Value(const string & value) : m_type(V_STRING)
{
    m_value = value;
}

Value::Value(const Value & value) : m_type(value.m_type)
{
    m_value = value.m_value;
}





Value & Value::operator=(bool value)
{
    m_type = V_BOOL;
    m_value = value ? "true" : "false";
    return *this;
}

Value & Value::operator=(int value)
{
    m_type = V_INT;
    m_value = std::to_string(value);
    return *this;
}

Value & Value::operator=(float value)
{
    m_type = V_FLOAT;
    m_value = std::to_string(value);
    return *this;
}

Value & Value::operator=(double value)
{
    m_type = V_DOUBLE;
    m_value = std::to_string(value);
    return *this;
}

Value & Value::operator=(const char * value)
{
    m_type = V_STRING;
    m_value = value;
    return *this;
}

Value & Value::operator=(const string & value)
{
    m_type = V_STRING;
    m_value = value;
    return *this;
}

Value & Value::operator=(const Value & other)
{
    m_type = other.m_type;
    m_value = other.m_value;
    return *this;
}





Value::Type Value::type() const
{
    return m_type;
}

bool Value::is_null() const
{
    return V_NULL == m_type;
}

bool Value::is_bool() const
{
    return V_BOOL == m_type;
}

bool Value::is_int() const
{
    return V_INT == m_type;
}

bool Value::is_float() const
{
    return V_FLOAT == m_type;
}

bool Value::is_double() const
{
    return V_DOUBLE == m_type;
}

bool Value::is_string() const
{
    return V_STRING == m_type;
}





bool Value::operator==(const Value & other) const
{
    if (other.m_type != m_type)
    {
        return false;
    }
    return other.m_value == m_value;
}

bool Value::operator!=(const Value & other) const
{
    return !(other == *this);
}





Value::operator bool()
{
    return "true" == m_value;
}
Value::operator bool() const
{
    return "true" == m_value;
}



Value::operator int()
{
    std::stringstream ss;
    ss << m_value;
    int value = 0;
    ss >> value;
    return value;
}
Value::operator int() const
{
    std::stringstream ss;
    ss << m_value;
    int value = 0;
    ss >> value;
    return value;
}



Value::operator float()
{
    std::stringstream ss;
    ss << m_value;
    float value = 0.0;
    ss >> value;
    return value;
}
Value::operator float() const
{
    std::stringstream ss;
    ss << m_value;
    float value = 0.0;
    ss >> value;
    return value;
}



Value::operator double()
{
    std::stringstream ss;
    ss << m_value;
    double value = 0.0;
    ss >> value;
    return value;
}
Value::operator double() const
{
    std::stringstream ss;
    ss << m_value;
    double value = 0.0;
    ss >> value;
    return value;
}



Value::operator string()
{
    return m_value;
}
Value::operator string() const
{
    return m_value;
}





void Value::show() const
{
    string type;
    switch (m_type)
    {
        case V_NULL:
        {
            type = "null";
        }
            break;
        case V_BOOL:
        {
            type = "bool";
        }
            break;
        case V_INT:
        {
            type = "int";
        }
            break;
        case V_FLOAT:
        {
            type = "float";
        }
            break;
        case V_DOUBLE:
        {
            type = "double";
        }
            break;
        case V_STRING:
        {
            type = "string";
        }
            break;
        default:
            break;
    }
    std::cout << "type=" << type << ",value=" << m_value << std::endl;
}