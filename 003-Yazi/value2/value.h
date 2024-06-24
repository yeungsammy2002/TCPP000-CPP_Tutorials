#pragma once

#include <string>
#include <stdexcept>
#include <sstream>
#include <iostream>

using std::string;

namespace demo {
namespace utilities {

class Value
{
public:
    enum Type
    {
        V_NULL = 0,
        V_BOOL,
        V_INT,
        V_FLOAT,
        V_DOUBLE,
        V_STRING,
    };

    Value() = default;
    ~Value() = default;

    Value(bool value);
    Value(int value);
    Value(float value);
    Value(double value);
    Value(const char * value);
    Value(const string & value);
    Value(const Value & other);

    Value & operator=(const Value & other);

    bool is_null() const { return V_NULL == m_type; }
    bool is_bool() const { return V_BOOL == m_type; }
    bool is_int() const { return V_INT == m_type; }
    bool is_float() const { return V_FLOAT == m_type; }
    bool is_double() const { return V_DOUBLE == m_type; }
    bool is_string() const { return V_STRING == m_type; }

    Type type() const { return m_type; }

    operator bool() const;
    operator int() const;
    operator float() const;
    operator double() const;
    operator string() const;

    bool operator==(const Value & other) const;
    bool operator!=(const Value & other) const;

    friend std::ostream & operator<<(std::ostream & os, const Value & value);
    void show() const;

private:
    void copy(const Value & other);

private:
    Type m_type = V_NULL;
    string m_value = "";
};

}
}