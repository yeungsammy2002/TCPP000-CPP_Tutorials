#include <utilities/file.h>
#include <utilities/directory.h>





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
    size_t last = m_path.find_last_of(sep);
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
    return unlink(m_path.c_str()) == 0;
}

bool File::rename(const string & path)
{
    int ret = std::rename(m_path.c_str(), path.c_str());
    if (ret != 0)
    {
        return false;
    }
    m_path = Directory::normalize_path(path);
    return true;
}

void File::clear()
{
    std::ofstream ofs(m_path, std::ios::out);
    ofs.flush();
    ofs.close();
}

bool File::exists()
{
    std::ifstream ifs(m_path);
    return ifs.good();
}

bool File::copy(const string & path)
{
    std::ifstream ifs(m_path);
    if (ifs.fail())
    {
        return false;
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
    string data;
    int line = 0;
    while (std::getline(ifs, data))
    {
        ++line;
    }
    return line;
}

long File::size() const
{
    // method 1
//    std::ifstream ifs(m_path);
//    ifs.seekg(0, std::ios_base::end);
//    return (long) ifs.tellg();

    // method 2
    struct stat info{ 0 };
    if (stat(m_path.c_str(), &info) != 0)
    {
        std::cerr << "stat file error: " << m_path << std::endl;
        return 0;
    }
    return info.st_size;
}

time_t File::time() const
{
    struct stat info{ 0 };
    if (stat(m_path.c_str(), &info) != 0)
    {
        std::cerr << "stat file error: " << m_path << std::endl;
        return 0;
    }
    return info.st_mtime;
}

string File::read() const
{
    // method 1
//    std::ifstream ifs(m_path);
//    std::ostringstream oss;
//    string data;
//    while (std::getline(ifs, data))
//    {
//        oss << data;
//    }
//    return oss.str();

    // method 2
    std::ifstream ifs(m_path);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

bool File::write(const string & data)
{
    std::ofstream ofs(m_path);
    if (!ofs.is_open())
    {
        return false;
    }
    ofs.write(data.c_str(), data.size());
    return true;
}

