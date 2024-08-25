#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <cassert>

using std::string;

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

    Json() = default;
    ~Json();

    Json(Type type);
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char * value);
    Json(const string & value);
    Json(const Json & other);
    Json(Json && other);

    bool is_str_null() const { return m_value.m_string == nullptr; }
    bool is_arr_null() const { return m_value.m_array == nullptr; }
    bool is_obj_null() const { return m_value.m_object == nullptr; }

    string str() const;
    friend std::ostream & operator<<(std::ostream & os, const Json & value);

    Type type() const { return m_type; }
    bool is_null() const { return m_type == JSON_NULL; }
    bool is_bool() const { return m_type == JSON_BOOL; }
    bool is_int() const { return m_type == JSON_INT; }
    bool is_double() const { return m_type == JSON_DOUBLE; }
    bool is_string() const { return m_type == JSON_STRING; }
    bool is_array() const { return m_type == JSON_ARRAY; }
    bool is_object() const { return m_type == JSON_OBJECT; }

    bool as_bool() const;
    int as_int() const;
    double as_double() const;
    string as_string() const;

    operator bool() const { return as_bool(); }
    operator int() const { return as_int(); }
    operator double() const { return as_double(); }
    operator string() const { return as_string(); }

    Json & operator=(bool value);
    Json & operator=(int value);
    Json & operator=(double value);
    Json & operator=(const char * value);
    Json & operator=(const string & value);
    Json & operator=(const Json & other);
    Json & operator=(Json && other);

    int size() const;
    bool empty() const;

    void append(const Json & value);
    void append(Json && value);
    bool has(const int index) const;
    Json get(const int index) const;
    Json & operator[](const int index);
    void remove(const int index);

    bool has(const char * key) const;
    bool has(const string & key) const;
    Json get(const char * key) const;
    Json get(const string & key) const;
    Json & operator[](const char * key);
    Json & operator[](const string & key);
    void remove(const char * key);
    void remove(const string & key);

    typedef std::vector<Json>::iterator iterator;
    iterator begin() const noexcept { return m_value.m_array->begin(); }
    iterator end() const noexcept { return m_value.m_array->end(); }

    void parse(const string & filename);
    void parse(const char * buf, int len);

private:
    void clear();
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

    Type m_type = JSON_NULL;
    Value m_value;
};

