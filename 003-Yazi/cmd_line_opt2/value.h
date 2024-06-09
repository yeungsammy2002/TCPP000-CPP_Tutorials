#pragma once

#include <string>
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
        V_STRING
    };

    Value();
    Value(bool value);
    Value(int value);
    Value(float value);
    Value(double value);
    Value(const char * value);
    Value(const string & value);
    Value(const Value & value);
    ~Value() = default;

    Value & operator=(bool value);
    Value & operator=(int value);
    Value & operator=(float value);
    Value & operator=(double value);
    Value & operator=(const char * value);
    Value & operator=(const string & value);
    Value & operator=(const Value & other);

    Type type() const;
    bool is_null() const;
    bool is_bool() const;
    bool is_int() const;
    bool is_float() const;
    bool is_double() const;
    bool is_string() const;

    bool operator==(const Value & other) const;
    bool operator!=(const Value & other) const;

    operator bool();
    operator bool() const;

    operator int();
    operator int() const;

    operator float();
    operator float() const;

    operator double();
    operator double() const;

    operator string();
    operator string() const;

    friend std::ostream & operator<<(std::ostream & os, const Value & value)
    {
        os << value.m_value;
        return os;
    }

    void show() const;

private:
    Type m_type;
    string m_value;
};

}
}

