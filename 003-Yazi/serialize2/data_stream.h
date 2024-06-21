
#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <map>
#include <list>
#include <set>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <serialize/serializable.h>

using std::string;

namespace demo {
namespace serialize {

class DataStream
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
        LIST,
        MAP,
        SET,
        CUSTOM
    };

    enum ByteOrder
    {
        BigEndian = 0,
        LittleEndian
    };

    DataStream();
    DataStream(const string & data);
    ~DataStream() = default;



    void write(const char * data, int len);

    void write(bool value);
    void write(char value);
    void write(int32_t value);
    void write(int64_t value);
    void write(float value);
    void write(double value);
    void write(const char * value);
    void write(const string & value);

    void write(const Serializable & value);

    template<typename T>
    void write(const std::vector<T> & value);

    template<typename T>
    void write(const std::list<T> & value);

    template<typename K, typename V>
    void write(const std::map<K, V> & value);

    template<typename T>
    void write(const std::set<T> & value);

    template<typename T, typename ...Args>
    void write_args(const T & head, const Args & ... args);

    void write_args() {};


    DataStream & operator<<(bool value);
    DataStream & operator<<(char value);
    DataStream & operator<<(int32_t value);
    DataStream & operator<<(int64_t value);
    DataStream & operator<<(float value);
    DataStream & operator<<(double value);
    DataStream & operator<<(const char * value);
    DataStream & operator<<(const string & value);
    DataStream & operator<<(const Serializable & value);

    template<typename T>
    DataStream & operator<<(const std::vector<T> & value);

    template<typename T>
    DataStream & operator<<(const std::list<T> & value);

    template<typename K, typename V>
    DataStream & operator<<(const std::map<K, V> & value);

    template<typename T>
    DataStream & operator<<(const std::set<T> & value);



    bool read(char * data, int len);

    bool read(bool & value);
    bool read(char & value);
    bool read(int32_t & value);
    bool read(int64_t & value);
    bool read(float & value);
    bool read(double & value);
    bool read(string & value);
    bool read(Serializable & value);

    template<typename T>
    bool read(std::vector<T> & value);

    template<typename T>
    bool read(std::list<T> & value);

    template<typename K, typename V>
    bool read(std::map<K, V> & value);

    template<typename T>
    bool read(std::set<T> & value);

    template<typename T, typename ...Args>
    bool read_args(T & head, Args & ...args);

    bool read_args() { return true; }

    DataStream & operator>>(bool & value);
    DataStream & operator>>(char & value);
    DataStream & operator>>(int32_t & value);
    DataStream & operator>>(int64_t & value);
    DataStream & operator>>(float & value);
    DataStream & operator>>(double & value);
    DataStream & operator>>(string & value);
    DataStream & operator>>(Serializable & value);

    template<typename T>
    DataStream & operator>>(std::vector<T> & value);

    template<typename T>
    DataStream & operator>>(std::list<T> & value);

    template<typename K, typename V>
    DataStream & operator>>(std::map<K, V> & value);

    template<typename T>
    DataStream & operator>>(std::set<T> & value);

    const char * data() const;
    int size() const;
    void clear();
    void reset();
    void save(const string & filename);
    void load(const string & filename);

    void show() const;

private:
    void reserve(int len);

    ByteOrder byteorder();

private:
    std::vector<char> m_buf;
    int m_pos;
    ByteOrder m_byteorder;
};

#include <serialize/data_stream.inl>

}
}