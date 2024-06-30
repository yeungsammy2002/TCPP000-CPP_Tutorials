#pragma once

#include <string>
#include <map>
#include <fstream>
#include <sstream>

#include <utilities/value.h>

using std::string;

namespace demo {
namespace utilities {

class Ini
{
    typedef std::map<string, Value> Section;

public:
    Ini() = default;
    ~Ini() = default;

    Ini(const string & filename);
    
    void clear();

    void load(const string & filename);
    void save(const string & filename);

    string str() const;
    void show() const;

    Value & get(const string & section, const string & option);
    void set(const string & section, const string & option, const Value & value);

    Section & operator[](const string & section);

    bool has(const string & section) const;
    bool has(const string & section, const string & option) const;

    void remove(const string & section);
    void remove(const string & section, const string & option);

private:
    string trim(string s);

private:
    std::map<string, Section> m_sections;
};

}
}