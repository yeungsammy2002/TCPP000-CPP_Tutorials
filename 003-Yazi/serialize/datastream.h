#pragma once

#include <vector>
#include <cstring>
#include <cstdint>
#include <string>
#include <iostream>

using std::string;

namespace demo {
namespace serialize {

class Datastream
{
public:
    enum DataType
    {
        BOOL = 0,
        CHAR,
        INT32,
        INT64,
        FLOAT,
        DOUBLE,
        STRING,
        VECTOR,
        MAP,
        LIST,
        SET,
        CUSTOM
    };

    Datastream();

    ~Datastream() = default;

    void write(const char * buff, int len);

    void write(bool value);

    void write(char value);

    void write(int32_t value);

    void write(int64_t value);

    void write(float value);

    void write(double value);

    void write(const char * value);

    void write(const string & value);

    Datastream & operator<<(bool value);

    Datastream & operator<<(char value);

    Datastream & operator<<(int32_t value);

    Datastream & operator<<(int64_t value);

    Datastream & operator<<(float value);

    Datastream & operator<<(double value);

    Datastream & operator<<(const char * value);

    Datastream & operator<<(const string & value);

    void show() const;

    bool read(bool & value);

    bool read(char & value);

    bool read(int32_t & value);

    bool read(int64_t & value);

    bool read(float & value);

    bool read(double & value);

    bool read(string & value);

    Datastream & operator>>(bool & value);

    Datastream & operator>>(char & value);

    Datastream & operator>>(int32_t & value);

    Datastream & operator>>(int64_t & value);

    Datastream & operator>>(float & value);

    Datastream & operator>>(double & value);

    Datastream & operator>>(string & value);

private:
    void reserve(int len);

private:
    std::vector<char> m_buff;
    int m_pos;
};
}
}