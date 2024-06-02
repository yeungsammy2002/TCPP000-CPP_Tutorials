#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <memory>

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

    Type type() const;

    bool is_null() const;
    bool is_bool() const;
    bool is_int() const;
    bool is_double() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;

    bool as_bool() const;
    int as_int() const;
    double as_double() const;
    string as_string() const;

    void clear();

    Json & operator=(bool value);
    Json & operator=(int value);
    Json & operator=(double value);
    Json & operator=(const char * value);
    Json & operator=(const string & value);

    Json & operator=(const Json & other);

    Json & operator=(Json && other) noexcept;

    operator bool();
    operator bool() const;
    operator int();
    operator int() const;
    operator double();
    operator double() const;
    operator string();
    operator string() const;

    string str() const;

    void show() const;

    friend std::ostream & operator<<(std::ostream & os, Json & value)
    {
        os << value.str();
        return os;
    }

    void append(const Json & value);
    void append(Json && value);

    bool has(int index);
    bool has(const char * key) const;
    bool has(const string & key) const;

    Json get(int index);
    Json get(const char * key);
    Json get(const string & key);

    void remove(int index);
    void remove(const char * key);
    void remove(const string & key);

    Json & operator[](int index);
    Json & operator[](const char * key);
    Json & operator[](const string & key);

    typedef std::vector<Json>::iterator iterator;

    iterator begin()
    {
        return m_value.m_array->begin();
    }

    iterator end()
    {
        return m_value.m_array->end();
    }

    int size() const;

    bool empty() const;

    void parse(const string & filename);
    void parse(const char * buff, int len);

private:
    void copy(const Json & other);
    void swap(Json & other);

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