#pragma once

#include <string>
#include <map>
#include <list>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <utilities/value.h>

using namespace demo::utilities;

using std::string;

namespace demo {
namespace xml {

class Xml
{
public:
    Xml();

    Xml(const string & name);

    Xml(const Xml & other);

    ~Xml() = default;

    typedef std::list<Xml>::iterator iterator;

    iterator begin();

    iterator end();

    iterator erase(iterator it);

    iterator begin() const;

    iterator end() const;

    void clear();

    Xml & operator=(const Xml & other);

    string name() const;

    void name(const string & name);

    string text() const;

    void text(const string & text);

    Value & attr(const string & key);

    void attr(const string & key, const Value & value);

    string str() const;

    void show() const;

    void append(const Xml & child);

    int size() const;

    Xml & operator[](const int index);

    Xml & operator[](const char * name);

    Xml & operator[](const string & name);

    void remove(const int index);

    void remove(const char * name);

    void remove(const string & name);

private:
    void copy(const Xml & other);

private:
    string * m_name;
    string * m_text;
    std::map<string, Value> * m_attrs;
    std::list<Xml> * m_child;
};

}
}