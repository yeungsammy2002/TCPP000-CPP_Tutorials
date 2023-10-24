#include <IniFile.h>

Value::Value(bool value)
{
    *this = value;
}

Value::Value(int value)
{
    *this = value;
}

Value::Value(double value)
{
    *this = value;
}

Value::Value(const char * value)
{
    *this = value;
}

Value::Value(const string & value)
{
    *this = value;
}

Value & Value::operator=(bool value)
{
    m_value = value ? "true" : "false";
    return *this;
}

Value & Value::operator=(int value)
{
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value & Value::operator=(double value)
{
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value & Value::operator=(const char * value)
{
    m_value = value;
    return *this;
}

Value & Value::operator=(const string & value)
{
    m_value = value;
    return *this;
}

Value::operator bool()
{
    return "true" == m_value;
}

Value::operator int()
{
    return std::atoi(m_value.c_str());
}

Value::operator double()
{
    return std::atof(m_value.c_str());
}

Value::operator string()
{
    return m_value;
}

bool IniFile::load(const string & filename)
{
    m_filename = filename;
    std::ifstream fin(filename);
    if (fin.fail())
        return false;

    string line;
    string section;
    while (std::getline(fin, line))
    {
        line = trim(line);
        if ("" == line)
            continue;

        if ('[' == line[0])
        {
            int pos = line.find_first_of("]");
            section = line.substr(1, pos - 1);
            section = trim(section);
            m_sections[section] = Section();
        } else
        {
            int pos = line.find_first_of("=");
            string key = line.substr(0, pos);
            key = trim(key);
            string value = line.substr(pos + 1, line.length() - pos);
            value = trim(value);
            m_sections[section][key] = value;
        }
    }
    fin.close();
    return true;
}

string IniFile::trim(string s)
{
    if (s.empty())
        return s;
    s.erase(0, s.find_first_not_of(" \n\r"));
    s.erase(s.find_last_not_of(" \n\r") + 1);
    return s;
}

Value & IniFile::get(const string & section, const string & key)
{
    return m_sections[section][key];
}

Section & IniFile::operator[](const string & section)
{
    return m_sections[section];
}

void IniFile::set(const string & section)
{
    m_sections[section] = Section();
}

void IniFile::set(const string & section, const string & key, const Value & value)
{
    m_sections[section][key] = value;
}

bool IniFile::has(const string & section)
{
    return m_sections.find(section) != m_sections.end();
}

bool IniFile::has(const string & section, const string & key)
{
    if (!has(section))
        return false;
    return m_sections[section].find(key) != m_sections[section].end();
}

void IniFile::remove(const string & section)
{
    if (has(section))
        m_sections.erase(section);
}

void IniFile::remove(const string & section, const string & key)
{
    if (!has(section))
        return;
    if (has(section, key))
        m_sections[section].erase(key);
}

void IniFile::clear()
{
    m_sections.clear();
}

string IniFile::str()
{
    std::stringstream ss;
    for (auto it = m_sections.begin(); it != m_sections.end(); ++it)
    {
        ss << '[' << it->first << ']' << std::endl;
        for (auto itr = (it->second).begin(); itr != (it->second).end(); ++itr)
        {
            ss << itr->first << " = " << string(itr->second) << std::endl;
        }
        ss << std::endl;
    }
    return ss.str();
}

void IniFile::show()
{
    std::cout << str() << std::endl;
}

bool IniFile::save(const string & filename)
{
    std::ofstream fout(filename);
    if (fout.fail())
        return false;
    fout << str();
    fout.close();
    return true;
}