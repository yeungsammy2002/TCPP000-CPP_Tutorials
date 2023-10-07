#include <json/json.h>

using namespace demo::json;

Json::Json() : m_type(json_null) {}

Json::Json(bool value) : m_type(json_bool) {
    m_value.m_bool = value;
}

Json::Json(int value) : m_type(json_int) {
    m_value.m_int = value;
}

Json::Json(double value) : m_type(json_double) {
    m_value.m_double = value;
}

Json::Json(const char* value) : m_type(json_string) {
    m_value.m_string = new std::string(value);
}

Json::Json(const std::string& value) : m_type(json_string) {
    m_value.m_string = new std::string(value);
}

Json::Json(Type type) : m_type(type) {
    switch (m_type) {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_string:
        m_value.m_string = new std::string("");
        break;
    case json_array:
        m_value.m_array = new std::vector<Json>();
        break;
    case json_object:
        m_value.m_object = new std::map<std::string, Json>();
        break;
    default:
        break;
    }
}

Json::Json(const Json& other) {
    copy(other);
}

void Json::operator=(const Json& other) {
    clear();
    copy(other);
}

Json::operator bool() {
    if (m_type != json_bool)
        throw new std::logic_error("type error, not a bool value");
    return m_value.m_bool;
}

Json::operator int() {
    if (m_type != json_int)
        throw new std::logic_error("type error, not an int value");
    return m_value.m_int;
}

Json::operator double() {
    if (m_type != json_double)
        throw new std::logic_error("type error, not a double value");
    return m_value.m_double;
}

Json::operator std::string() {
    if (m_type != json_string)
        throw new std::logic_error("type error, not a string value");
    return *(m_value.m_string);
}

Json& Json::operator[](int index) {
    if (m_type != json_array) {
        clear();
        m_type = json_array;
        m_value.m_array = new std::vector<Json>();
    }
    if (index < 0)
        throw new std::logic_error("array [] index < 0");
    int size = (m_value.m_array)->size();
    if (index >= size) {
        for (int i = size; i <= index; ++i) {
            (m_value.m_array)->push_back(Json());
        }
    }
    return (m_value.m_array)->at(index);
}

void Json::append(const Json& other) {
    if (m_type != json_array) {
        clear();
        m_type = json_array;
        m_value.m_array = new std::vector<Json>();
    }
    (m_value.m_array)->push_back(other);
}

std::string Json::str() const {
    std::stringstream ss;
    switch (m_type) {
    case json_null:
        ss << "null";
        break;
    case json_bool: {
        if (m_value.m_bool) {
            ss << "true";
        }
        else {
            ss << "false";
        }
        break;
    }
    case json_int:
        ss << m_value.m_int;
        break;
    case json_double:
        ss << m_value.m_double;
        break;
    case json_string:
        ss << '\"' << *(m_value.m_string) << '\"';
        break;
    case json_array: {
        ss << '[';
        for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); ++it) {
            if (it != (m_value.m_array)->begin())
                ss << ',';
            ss << it->str();
        }
        ss << ']';
        break;
    }
    case json_object: {
        ss << '{';
        for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); ++it) {
            if (it != (m_value.m_object)->begin())
                ss << ',';
            ss << '\"' << it->first << '\"' << ':' << (it->second).str();
        }
        ss << '}';
        break;
    }
    default:
        break;
    }
    return ss.str();
}

Json& Json::operator[](const char* key) {
    std::string name{ key };
    return (*this)[name];
}

Json& Json::operator[](const std::string& name) {
    if (m_type != json_object) {
        clear();
        m_type = json_object;
        m_value.m_object = new std::map<std::string, Json>();
    }
    return (*(m_value.m_object))[name];
}

Json::iterator Json::begin() {
    return (m_value.m_array)->begin();
}

Json::iterator Json::end() {
    return (m_value.m_array)->end();
}

bool Json::operator==(const Json& other) {
    if (m_type != other.m_type) return false;
    switch (m_type) {
    case json_null:
        return true;
    case json_bool:
        return m_value.m_bool == other.m_value.m_bool;
    case json_int:
        return m_value.m_int == other.m_value.m_int;
    case json_double:
        return m_value.m_double == other.m_value.m_double;
    case json_string:
        return *(m_value.m_string) == *(other.m_value.m_string);
    case json_array:
        return m_value.m_array == other.m_value.m_array;
    case json_object:
        return m_value.m_object == other.m_value.m_object;
    default:
        return false;
    }
}

bool Json::operator!=(const Json& other) {
    return !(*this == other);
}

bool Json::asBool() const {
    if (m_type != json_bool)
        throw new std::logic_error("type error, not a bool value");
    return m_value.m_bool;
}

int Json::asInt() const {
    if (m_type != json_int)
        throw new std::logic_error("type error, not an int value");
    return m_value.m_int;
}

double Json::asDouble() const {
    if (m_type != json_double)
        throw new std::logic_error("type error, not a double value");
    return m_value.m_double;
}

std::string Json::asString() const {
    if (m_type != json_string)
        throw new std::logic_error("type error, not a string value");
    return *(m_value.m_string);
}

bool Json::has(int index) const {
    if (m_type != json_array) return false;
    int size = (m_value.m_array)->size();
    return (index >= 0 && index < size);
}

bool Json::has(const char* key) const {
    std::string name{ key };
    return has(name);
}

bool Json::has(const std::string& name) const {
    if (m_type != json_object) return false;
    auto it = (m_value.m_object)->end();
    return ((m_value.m_object)->find(name) != it);
}

void Json::copy(const Json& other) {
    m_type = other.m_type;
    switch (m_type) {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = other.m_value.m_bool;
        break;
    case json_int:
        m_value.m_int = other.m_value.m_int;
        break;
    case json_double:
        m_value.m_double = other.m_value.m_double;
        break;
    case json_string:
        m_value.m_string = other.m_value.m_string;
        break;
    case json_array:
        m_value.m_array = other.m_value.m_array;
        break;
    case json_object:
        m_value.m_object = other.m_value.m_object;
        break;
    default:
        break;
    }
}

void Json::clear() {
    switch (m_type) {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_string:
        delete m_value.m_string;
        break;
    case json_array: {
        for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); ++it) {
            it->clear();
        }
        delete m_value.m_array;
        break;
    }
    case json_object: {
        for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); ++it) {
            (it->second).clear();
        }
        delete m_value.m_object;
        break;
    }
    default:
        break;
    }
    m_type = json_null;
}

void Json::remove(int index) {
    if (!has(index)) return;
    (*this)[index].clear();
    auto it = (m_value.m_array)->begin();
    (m_value.m_array)->erase(it + index);
}

void Json::remove(const char* key) {
    std::string name{ key };
    remove(name);
}

void Json::remove(const std::string& name) {
    if (!has(name)) return;
    (*this).clear();
    (m_value.m_object)->erase(name);
}