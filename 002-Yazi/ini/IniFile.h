#pragma once

#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;

class Value
{
public:
    Value() = default;

    ~Value() = default;

    Value(bool value);

    Value(int value);

    Value(double value);

    Value(const char * value);

    Value(const string & value);

    Value & operator=(bool value);

    Value & operator=(int value);

    Value & operator=(double value);

    Value & operator=(const char * value);

    Value & operator=(const string & value);

    operator bool();

    operator int();

    operator double();

    operator string();

private:
    string m_value;
};

typedef std::map<string, Value> Section;

class IniFile
{
public:
    IniFile() = default;

    ~IniFile() = default;

    bool load(const string & filename);

    string trim(string s);

    Value & get(const string & section, const string & key);

    Section & operator[](const string & section);

    void set(const string & section);

    void set(const string & section, const string & key, const Value & value);

    bool has(const string & section);

    bool has(const string & section, const string & key);

    void remove(const string & section);

    void remove(const string & section, const string & key);

    void clear();

    string str();

    void show();

    bool save(const string & filename);

private:
    string m_filename;
    std::map<string, Section> m_sections;
};