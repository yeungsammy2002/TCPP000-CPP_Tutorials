#pragma once

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <sstream>

using std::string;

namespace demo {
    namespace json {

        class Json
        {
        public:
            enum Type
            {
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

            Json(const char* value);

            Json(const string& value);

            Json(Type type);

            Json(const Json& other);

            bool is_null() const { return json_null == m_type; }

            bool is_bool() const { return json_bool == m_type; }

            bool is_int() const { return json_int == m_type; }

            bool is_double() const { return json_double == m_type; }

            bool is_string() const { return json_string == m_type; }

            bool is_array() const { return json_array == m_type; }

            bool is_object() const { return json_object == m_type; }

            operator bool();

            operator int();

            operator double();

            operator string();

            void clear();

            Json& operator[](const int index);

            void append(const Json& other);

            int size() const;

            string str() const;

            Json& operator[](const char* key);

            Json& operator[](const string& key);

            Json& operator=(const Json& other);

            typedef std::vector<Json>::iterator iterator;

            iterator begin() const;

            iterator end() const;

            iterator erase(iterator it);

            bool operator==(const Json& other) const;

            bool operator!=(const Json& other) const;

            bool as_bool() const;

            int as_int() const;

            double as_double() const;

            string as_string() const;

            bool has(const int index) const;

            bool has(const char* key) const;

            bool has(const string& key) const;

            void remove(const int index);

            void remove(const char* key);

            void remove(const string& key);

        private:
            void copy(const Json& other);

        private:
            union Value
            {
                bool m_bool;
                int m_int;
                double m_double;
                string* m_string;
                std::vector<Json>* m_array;
                std::map<string, Json>* m_object;
            };

            Type m_type;
            Value m_value;
        };

    }
}