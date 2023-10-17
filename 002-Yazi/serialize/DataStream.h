#pragma once

#include <vector>
#include <cstring>
#include <cstdint>
#include <string>
#include <iostream>

using namespace std;

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

            DataStream() : m_pos(0) {}

            ~DataStream() = default;

            void write(const char * data, int len);

            void write(bool value);

            void write(char value);

            void write(std::int32_t value);

            void write(std::int64_t value);

            void write(float value);

            void write(double value);

            void write(const char * value);

            void write(const string & value);

            void show() const;

            DataStream & operator<<(bool value);

            DataStream & operator<<(char value);

            DataStream & operator<<(std::int32_t value);

            DataStream & operator<<(std::int64_t value);

            DataStream & operator<<(float value);

            DataStream & operator<<(double value);

            DataStream & operator<<(const char * value);

            DataStream & operator<<(const string & value);

            bool read(bool & value);

            bool read(char & value);

            bool read(std::int32_t & value);

            bool read(std::int64_t & value);

            bool read(float & value);

            bool read(double & value);

            bool read(string & value);

            DataStream & operator>>(bool & value);

            DataStream & operator>>(char & value);

            DataStream & operator>>(std::int32_t & value);

            DataStream & operator>>(std::int64_t & value);

            DataStream & operator>>(float & value);

            DataStream & operator>>(double & value);

            DataStream & operator>>(string & value);

        private:
            void reserve(int len);

        private:
            std::vector<char> m_buf;
            int m_pos;
        };
    }
}