#include <utilities/ini.h>

using namespace demo::utilities;





Ini::Ini(const string & filename)
{
    load(filename);
}





void Ini::clear()
{
    m_sections.clear();
}





void Ini::load(const string & filename)
{
    clear();

    std::ifstream ifs(filename);
    if (ifs.fail())
    {
        throw std::logic_error("loading ini file failed: " + filename);
    }

    string line;
    string section;

    while (std::getline(ifs, line))
    {
        line = trim(line);
        if (line.empty())
        {
            continue;
        } else if ('#' == line[0] || ';' == line[0])
        {
            continue;
        } else if ('[' == line[0])
        {
            const auto pos = line.find_first_of("]");
            if (pos == std::string::npos)
            {
                throw std::logic_error("invalid section: " + line);
            }
            section = line.substr(1, pos - 1);
            section = trim(section);
            m_sections[section] = Section();
        } else
        {
            const auto pos = line.find_first_of("=");
            if (pos == std::string::npos)
            {
                throw std::logic_error("invalid option: " + line);
            }
            string option = line.substr(0, pos);
            option = trim(option);
            string value = line.substr(pos + 1);
            value = trim(value);
            m_sections[section][option] = value;
        }
    }
    ifs.close();
}

void Ini::save(const string & filename)
{
    std::ofstream ofs(filename);
    if (ofs.fail())
    {
        throw std::logic_error("saving ini file failed: " + filename);
    }
    ofs << str();
    ofs.flush();
    ofs.close();
}





string Ini::str() const
{
    std::ostringstream oss;
    for (const auto & section: m_sections)
    {
        oss << "[" << section.first << "]\n";
        for (const auto & option: section.second)
        {
            oss << option.first << " = " << option.second << "\n";
        }
        oss << "\n";
    }
    return oss.str();
}

void Ini::show() const
{
    std::cout << str() << std::endl;
}





Value & Ini::get(const string & section, const string & option)
{
    return m_sections[section][option];
}

void Ini::set(const string & section, const string & option, const Value & value)
{
    m_sections[section][option] = value;
}





Ini::Section & Ini::Ini::operator[](const string & section)
{
    return m_sections[section];
}





bool Ini::has(const string & section) const
{
    return m_sections.find(section) != m_sections.end();
}

bool Ini::has(const string & section, const string & option) const
{
    const auto it = m_sections.find(section);
    if (it == m_sections.end())
    {
        return false;
    }
    return (it->second).find(option) != (it->second).end();
}





void Ini::remove(const string & section)
{
    if (!has(section))
    {
        return;
    }
    m_sections.erase(section);
}

void Ini::remove(const string & section, const string & option)
{
    if (!has(section, option))
    {
        return;
    }
    m_sections[section].erase(option);
}





string Ini::trim(string s)
{
    if (s.empty())
    {
        return s;
    }
    s.erase(0, s.find_first_not_of(" \n\r"));
    s.erase(s.find_last_not_of(" \n\r") + 1);
    return s;
}