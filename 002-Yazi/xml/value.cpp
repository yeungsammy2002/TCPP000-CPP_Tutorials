#include <utilities/value.h>

using namespace demo::utilities;

Value::Value(bool value)
{
    m_value = value ? "true" : "false";
}

Value::Value(int value)
{
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
}

Value::Value(double value)
{
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
}

Value::Value(const char * value)
{
    m_value = value;
}

Value::Value(const string & value)
{
    m_value = value;
}

Value::Value(const Value & other)
{
    m_value = other.m_value;
}

Value & Value::operator=(const Value & other)
{
    m_value = other.m_value;
    return *this;
}

bool Value::operator==(const Value & other) const
{
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

Value::operator int()
{
    std::stringstream ss;
    ss << m_value;
    int value = 0;
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

Value::operator string()
{
    return m_value;
}

