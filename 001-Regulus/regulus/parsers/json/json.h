#pragma once

#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <sstream>
#include <iostream>

using std::string;

namespace regulus {
namespace parsers {

class Json
{
public:
    enum Type
    {
        JSON_NULL = 0,
        JSON_BOOL,
        JSON_INT,
        JSON_DOUBLE,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT
    };

    ~Json();

    Json();
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char * value);
    Json(const string & value);

    bool is_null() const { return JSON_NULL == m_type; }
    bool is_bool() const { return JSON_BOOL == m_type; }
    bool is_int() const { return JSON_INT == m_type; }
    bool is_double() const { return JSON_DOUBLE == m_type; }
    bool is_string() const { return JSON_STRING == m_type; }
    bool is_array() const { return JSON_ARRAY == m_type; }
    bool is_object() const { return JSON_OBJECT == m_type; }

    operator bool() const;
    operator int() const;
    operator double() const;
    operator string() const;

    Json(Type type);

    Json(const Json & other);
    Json & operator=(const Json & other);

    string str() const;
    void show() const;

    void append(const Json & value);

private:
    void copy(const Json & other);

    void yazi_cpy(const Json & other);

    void clear();

private:
    union Value
    {
        bool m_bool;
        int m_int;
        double m_double;
        string * m_string;
        std::vector<Json> * m_array;
        std::map<string, Json> * m_object;
    };

    Type m_type;
    Value m_value;
};

}
}