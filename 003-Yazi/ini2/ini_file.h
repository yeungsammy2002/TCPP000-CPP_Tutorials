#pragma once

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>

#include <utilities/value.h>

using std::string;

namespace demo {
namespace utilities {

class IniFile
{
    typedef std::map<string, Value> Section;
public:
    IniFile() = default;
    IniFile(const string & filename);
    ~IniFile() = default;

    bool load(const string & filename);
    void save(const string & filename);

    string str() const;
    void show() const;

    Value & get(const string & section, const string & key);
    void set(const string & section, const string & key, const Value & val);

    Section & operator[](const string & section)
    {
        return m_sections[section];
    }

    bool has(const string & section);
    bool has(const string & section, const string & key);

    void remove(const string & section);
    void remove(const string & section, const string & key);

    void clear();
    
private:
    string trim(string s);

private:
    string m_filename;
    std::map<string, Section> m_sections;
};

}
}