#include <xml/Xml.h>

using namespace demo::xml;

Value::Value() {}

Value::Value(bool value) {
    *this = value;
}

Value::Value(int value) {
    *this = value;
}

Value::Value(double value) {
    *this = value;
}

Value::Value(const char * value) : m_value(value) {}

Value::Value(const std::string & value) : m_value(value) {}

Value::~Value() {}

Value & Value::operator=(bool value) {
    m_value = value ? "true" : "false";
    return *this;
}

Value & Value::operator=(int value) {
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value & Value::operator=(double value) {
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value & Value::operator=(const char * value) {
    m_value = value;
    return *this;
}

Value & Value::operator=(const std::string & value) {
    m_value = value;
    return *this;
}

Value & Value::operator=(const Value & value) {
    m_value = value.m_value;
    return *this;
}

bool Value::operator==(const Value & other) {
    return m_value == other.m_value;
}

bool Value::operator!=(const Value & other) {
    return !(*this == other);
}

Value::operator bool() {
    if ("true" == m_value)
        return true;
    else if ("false" == m_value)
        return false;
    return false;
}

Value::operator int() {
    return std::atoi(m_value.c_str());
}

Value::operator double() {
    return std::atof(m_value.c_str());
}

Value::operator std::string() {
    return m_value;
}

Xml::Xml() : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr) {
}

Xml::Xml(const char * name) : Xml(std::string(name)) {}

Xml::Xml(const std::string & name) : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr) {
    m_name = new std::string(name);
}

Xml::Xml(const Xml & other) {
    m_name = other.m_name;
    m_text = other.m_text;
    m_attrs = other.m_attrs;
    m_child = other.m_child;
}

std::string Xml::name() const {
    if (nullptr == m_name) return "";
    return *m_name;
}

void Xml::name(const std::string & name) {
    if (nullptr != m_name) {
        delete m_name;
        m_name = nullptr;
    }
    m_name = new std::string(name);
}

std::string Xml::text() const {
    if (nullptr == m_text) return "";
    return *m_text;
}

void Xml::text(const std::string & text) {
    if (nullptr != m_text) {
        delete m_text;
        m_text = nullptr;
    }
    m_text = new std::string(text);
}

Value & Xml::attr(const std::string & key) {
    if (nullptr == m_attrs) m_attrs = new std::map<std::string, Value>();
    return (*m_attrs)[key];
}

void Xml::attr(const std::string & key, const Value & val) {
    if (nullptr == m_attrs) m_attrs = new std::map<std::string, Value>();
    (*m_attrs)[key] = val;
}

std::string Xml::str() const {
    if (nullptr == m_name)
        throw std::logic_error("element name is empty");
    std::stringstream ss;
    ss << "<" << *m_name;
    if (nullptr != m_attrs) {
        for (auto it = m_attrs->begin(); it != m_attrs->end(); ++it) {
            ss << " " << it->first << "=\"" << std::string(it->second) << "\"";
        }
    }
    ss << ">";
    if (nullptr != m_child) {
        for (auto it = m_child->begin(); it != m_child->end(); ++it) {
            ss << it->str();
        }
    }
    if (nullptr != m_text)
        ss << *m_text;
    ss << "</" << *m_name << ">";
    return ss.str();
}

void Xml::clear() {
    if (nullptr != m_name) {
        delete m_name;
        m_name = nullptr;
    }
    if (nullptr != m_attrs) {
        delete m_attrs;
        m_attrs = nullptr;
    }
    if (nullptr != m_text) {
        delete m_text;
        m_text = nullptr;
    }
    if (nullptr != m_child) {
        for (auto it = m_child->begin(); it != m_child->end(); ++it) {
            it->clear();
        }
        delete m_child;
        m_child = nullptr;
    }
}


void Xml::append(const Xml & child) {
    if (nullptr == m_child) m_child = new std::list<Xml>();
    m_child->push_back(child);
}

Xml & Xml::operator[](int index) {
    if (index < 0)
        throw std::logic_error("index less than zero");
    if (nullptr == m_child) m_child = new std::list<Xml>();
    int size = m_child->size();
    auto it = m_child->begin();
    if (index < size) {
        for (int i = 0; i < index; ++i)
            ++it;
        return *it;
    }
    if (index >= size) {
        for (int i = size; i <= index; ++i) {
            m_child->push_back(Xml());
        }
    }
    return m_child->back();
}

Xml & Xml::operator[](const char * name) {
    return (*this)[std::string(name)];
}

Xml & Xml::operator[](const std::string & name) {
    if (nullptr == m_child) m_child = new std::list<Xml>();
    for (auto it = m_child->begin(); it != m_child->end(); ++it) {
        if (name == it->name())
            return *it;
    }
    m_child->push_back(Xml(name));
    return m_child->back();
}


void Xml::remove(int index) {
    if (nullptr == m_child) return;
    int size = m_child->size();
    if (index < 0 || index >= size) return;
    auto it = m_child->begin();
    for (int i = 0; i < index; ++i)
        it++;
    it->clear();
    m_child->erase(it);
}

void Xml::remove(const char * name) {
    remove(std::string(name));
}

void Xml::remove(const std::string & name) {
    if (nullptr == m_child) return;
    for (auto it = m_child->begin(); it != m_child->end();) {
        if (name == it->name()) {
            it->clear();
            it = m_child->erase(it);
        } else
            it++;
    }
}

Xml & Xml::operator=(const Xml & other) {
    clear();
    m_name = other.m_name;
    m_text = other.m_text;
    m_attrs = other.m_attrs;
    m_child = other.m_child;
    return *this;
}