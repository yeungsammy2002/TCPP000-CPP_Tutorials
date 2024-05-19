#include <ini/ini.h>

using namespace demo::ini;

bool Ini::load(const string & filename)
{
    m_filename = filename;
    std::ifstream fin(filename);
    if (fin.fail())
    {
        std::printf("open file failed: %s", filename.c_str());
        return false;
    }
    string line;
    string section;
    while (std::getline(fin, line))
    {
        line = trim(line);
        if ("" == line)
        {
            continue;
        }
        if ('[' == line[0])
        {
            section = line.substr(1, line.find_first_of("]") - 1);
        } else
        {
            const auto pos = line.find_first_of("=");
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            key = trim(key);
            value = trim(value);
            m_sections[section][key] = value;
        }
    }
    return true;
}

string Ini::trim(string s)
{
    s.erase(0, s.find_first_not_of(" \n\r"));
    s.erase(s.find_last_not_of(" \n\r") + 1);
    return s;
}

string Ini::str() const
{
    std::stringstream ss;
    for (auto it = m_sections.begin(); it != m_sections.end(); ++it)
    {
        ss << "[" << it->first << "]\n";
        for (auto itr = (it->second).begin(); itr != (it->second).end(); ++itr)
        {
            ss << itr->first << " = " << string(itr->second) << std::endl;
        }
    }
    return ss.str();
}

void Ini::show() const
{
    std::cout << str() << std::endl;
}

Value & Ini::get(const string & section, const string & key)
{
    return m_sections[section][key];
}

Section & Ini::operator[](const string & section)
{
    return m_sections[section];
}

void Ini::set(const string & section, const string & key, const Value & value)
{
    m_sections[section][key] = value;
}

bool Ini::has(const string & section, const string & key) const
{
    const auto it = m_sections.find(section);
    if (it == m_sections.end())
    {
        return false;
    }
    return (it->second).find(key) != (it->second).end();
}

bool Ini::has(const string & section) const
{
    return m_sections.find(section) != m_sections.end();
}

void Ini::remove(const string & section, const string & key)
{
    if (!has(section, key))
    {
        return;
    }
    m_sections[section].erase(key);
}

void Ini::remove(const string & section)
{
    if (!has(section))
    {
        return;
    }
    m_sections.erase(section);
}

void Ini::clear()
{
    m_sections.clear();
}

bool Ini::save(const string & filename)
{
    std::ofstream fout(filename);
    if (fout.fail())
    {
        std::printf("open file failed: %s", filename.c_str());
        return false;
    }
    fout << str();
    fout.flush();
    fout.close();
    return true;
}