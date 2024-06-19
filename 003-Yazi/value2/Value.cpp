#include <utilities/value.h>

using namespace demo::utilities;





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

Value::Value(const Value & other)
{
    copy(other);
}





Value & Value::operator=(const Value & other)
{
    if (&other == this)
    {
        return *this;
    }
    copy(other);
    return *this;
}





Value::operator bool() const
{
    if (!is_bool())
    {
        throw std::logic_error("type is not V_BOOL");
    }
    return "true" == m_value;
}

Value::operator int() const
{
    if (!is_int())
    {
        throw std::logic_error("type is not V_INT");
    }
    std::stringstream ss;
    int value = 0;
    ss << m_value;
    ss >> value;
    return value;
}

Value::operator float() const
{
    if (!is_float())
    {
        throw std::logic_error("type is not V_FLOAT");
    }
    std::stringstream ss;
    float value = 0.0f;
    ss << m_value;
    ss >> value;
    return value;
}

Value::operator double() const
{
    if (!is_double())
    {
        throw std::logic_error("type is not V_DOUBLE");
    }
    std::stringstream ss;
    double value = 0.0;
    ss << m_value;
    ss >> value;
    return value;
}

Value::operator string() const
{
    if (!is_string())
    {
        throw std::logic_error("type is not V_STRING");
    }
    return m_value;
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





std::ostream & demo::utilities::operator<<(std::ostream & os, const Value & value)
{
    os << value.m_value;
    return os;
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
    std::cout << "type=" << type << ", value=" << m_value << std::endl;
}





void Value::copy(const Value & other)
{
    m_type = other.m_type;
    m_value = other.m_value;
}