#include <fs/file.h>

#include <fs/directory.h>

using namespace demo::fs;





File::File(const string & path)
{
    m_path = Directory::normalize_path(path);
}





string File::path() const
{
    return m_path;
}

string File::dir() const
{
    if (m_path.empty())
    {
        return "";
    }

    char sep = Directory::separator();

    const size_t last = m_path.find_last_of(sep);
    if (last != std::string::npos)
    {
        return m_path.substr(0, last);
    }
    return "";
}





bool File::create()
{
    if (exists())
    {
        return false;
    }
    Directory tmp(dir());
    if (!tmp.exists())
    {
        tmp.create();
    }
    std::ofstream ofs(m_path);
    return ofs.is_open();
}

bool File::remove()
{
    return 0 == unlink(m_path.c_str());     // defined in <unistd.h> (original in <cstdio>)
}

bool File::rename(const string & path)
{
    int ret = std::rename(m_path.c_str(), path.c_str());
    if (0 != ret)
    {
        return false;
    }
    m_path = Directory::normalize_path(path);
    return true;
}

void File::clear()
{
    std::ofstream ofs(m_path, std::ios::out);   // std::ios::out - remove all content within the file
    ofs.flush();
    ofs.close();
}

bool File::exists() const
{
    std::ifstream ifs(m_path);
    return ifs.good();                          // return `true` if the file already existed
}

bool File::copy(const string & path)
{
    std::ifstream ifs(m_path);
    if (ifs.fail())
    {
        return true;
    }

    File dst(path);
    if (!dst.exists())
    {
        dst.create();
    }
    
    std::ofstream ofs(path);
    if (ofs.fail())
    {
        return false;
    }

    ofs << ifs.rdbuf();
    ofs.flush();
    ofs.close();
    return true;
}

int File::line() const
{
    std::ifstream ifs(m_path);
    int line = 0;
    string data;
    while (std::getline(ifs, data))
    {
        ++line;
    }
    return line;
}

long File::size() const
{
    // 1st approach
//    std::ifstream ifs(m_path);
//    ifs.seekg(0, std::ios_base::end);         // move pointer to the end of the file
//    return (long) ifs.tellg();

    // 2nd approach
    struct stat info = { 0 };                   // stat defined in <sys/stat.h>
    if (0 != stat(m_path.c_str(), &info))
    {
        std::cerr << "stat file error: " << m_path << std::endl;
        return 0;
    }
    return info.st_size;
}

std::time_t File::time() const
{
    struct stat info = { 0 };                   // stat defined in <sys/stat.h>
    if (0 != stat(m_path.c_str(), &info))
    {
        std::cerr << "stat file error: " << m_path << std::endl;
        return 0;
    }
    return info.st_mtime;
}

string File::read() const
{
    // approach 1
//    std::ifstream ifs(m_path);
//    string data;
//    std::ostringstream oss;
//    while (std::getline(ifs, data))
//    {
//        oss << data;
//    }
//    return oss.str();

    // approach 2
    std::ifstream ifs(m_path);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

bool File::write(const string & data)
{
    std::ofstream ofs(m_path);  // override all content everytime we open document
    if (ofs.is_open())
    {
        return false;
    }
    ofs.write(data.c_str(), data.size());
    return true;
}

