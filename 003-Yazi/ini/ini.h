#pragma once

#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include <utilities/value.h>

using namespace demo::utilities;

namespace demo {
namespace ini {

typedef std::map<string, Value> Section;

class Ini
{
public:
    Ini() = default;

    ~Ini() = default;

    bool load(const string & filename);

    string str() const;

    void show() const;

    Value & get(const string & section, const string & key);

    Section & operator[](const string & section);

    void set(const string & section, const string & key, const Value & value);

    bool has(const string & section, const string & key) const;

    bool has(const string & section) const;

    void remove(const string & section, const string & key);

    void remove(const string & section);

    void clear();

    bool save(const string & filename);

private:
    string trim(string s);

private:
    string m_filename;
    std::map<string, Section> m_sections;
};

}
}