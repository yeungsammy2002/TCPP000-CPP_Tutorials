#include <utilities/directory.h>





Directory::Directory()
{
    m_path = ".";
}

Directory::Directory(const string& path)
{
    m_path = path;
}





string Directory::path() const
{
    return m_path;
}





char Directory::separator()
{
#ifdef WIN32
    return '\\';
#else
    return '/';
#endif
}

bool Directory::is_absolute_path(const string& path)
{
    if (path.empty())
    {
        return false;
    }
    char sep = Directory::separator();
    string filepath = path;

#ifdef WIN32
    std::replace(filepath.begin(), filepath.end(), '/', sep);
#endif

    std::vector<string> output = String::split(filepath, sep);

#ifdef WIN32
    if (output[0].find(':') != std::string::npos)
#else
    if (output[0].empty())
#endif
    {
        return true;
    }
    else
    {
        return false;
    }
}

// 00:16:25