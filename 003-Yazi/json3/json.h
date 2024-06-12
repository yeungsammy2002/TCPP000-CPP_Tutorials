#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <sstream>
#include <stdexcept>

using std::string;

namespace demo {
namespace json {

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

    Json();
    Json(Type type);
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char * value);
    Json(const string & value);
    Json(const Json & other);
    Json(Json && other);

    ~Json();

    Json & operator=(const Json & other);
    Json & operator=(Json && other) noexcept;

    bool is_null() const { return JSON_NULL == m_type; }
    bool is_bool() const { return JSON_BOOL == m_type; }
    bool is_int() const { return JSON_INT == m_type; }
    bool is_double() const { return JSON_DOUBLE == m_type; }
    bool is_string() const { return JSON_STRING == m_type; }
    bool is_array() const { return JSON_ARRAY == m_type; }
    bool is_object() const { return JSON_OBJECT == m_type; }

    bool as_bool() const;
    int as_int() const;
    double as_double() const;
    string as_string() const;

    operator bool() const { return as_bool(); }
    operator int() const { return as_int(); }
    operator double() const { return as_double(); }
    operator string() const { return as_string(); }

    void append(const Json & value);
    void append(Json && value);

    bool has(const int index) const;
    bool has(const char * key) const;
    bool has(const string & key) const;

    Json get(const int index) const;
    Json get(const char * key) const;
    Json get(const string & key) const;

    Json & operator[](const int index);
    Json & operator[](const char * key);
    Json & operator[](const string & key);

    void remove(const int index);
    void remove(const char * key);
    void remove(const string & key);

    string str() const;
    void show() const;
    friend std::ostream & operator<<(std::ostream & os, Json & value)
    {
        os << value.str();
        return os;
    }

    int size() const;
    bool empty() const;
    Type type() const;

    typedef std::vector<Json>::iterator iterator;
    iterator begin() { return m_value.m_array->begin(); }
    iterator end() { return m_value.m_array->end(); }
private:
    void copy(const Json & other);
    void move(Json && other);
    void swap(Json & other);
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

