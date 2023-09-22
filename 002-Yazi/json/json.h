#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>

namespace demo {
    namespace json {
        class Json {
        public:
            enum Type {
                json_null = 0,
                json_bool,
                json_int,
                json_double,
                json_string,
                json_array,
                json_object
            };

            Json();

            Json(bool value);

            Json(int value);

            Json(double value);

            Json(const char * value);

            Json(const std::string & value);

            Json(Type type);

            Json(const Json & other);

            operator bool();

            operator int();

            operator double();

            operator std::string();

            Json & operator[](int index);

            Json & operator[](const char * key);

            Json & operator[](const std::string & key);

            void operator=(const Json & other);

            void append(const Json & other);

            std::string str() const;

            void copy(const Json & other);

            void clear();

        private:
            union Value {
                bool m_bool;
                int m_int;
                double m_double;
                std::string * m_string;
                std::vector<Json> * m_array;
                std::map<std::string, Json> * m_object;
            };

            Type m_type;
            Value m_value;
        };
    }
}