#include <utilities/directory.h>





Directory::Directory()
{
    m_path = normalize_path(".");
}

Directory::Directory(const string& path)
{
    m_path = normalize_path(path);
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

string Directory::normalize_path(const string& path)
{
    char sep = Directory::separator();
    string filepath = path;

#ifdef WIN32
    std::replace(filepath.begin(), filepath.end(), '/', sep);
#endif

    std::vector<string> path_list;
    if (!is_absolute_path(path))
    {
        char cmd[PATH_MAX] = { 0 };
        getcwd(cmd, PATH_MAX);
        path_list = String::split(cmd, sep);
    }
    for (const auto& dir : String::split(filepath, sep))
    {
        if (dir.empty())
        {
            continue;
        }
        if (dir == ".")
        {
            continue;
        }
        else if (dir == "..")
        {
            path_list.pop_back();
        }
        else
        {
            path_list.push_back(dir);
        }
    }
    string temp = String::join(path_list, sep);
    return adjust_path(temp);
}

string Directory::adjust_path(const string& path)
{
    string result = path;
    char sep = separator();

#ifdef WIN32
    if (path[path.size() - 1] == ':')
    {
        result += sep;
    }
#else
    if (result[0] != sep)
    {
        result = string(1, sep) + result;
    }
#endif
    return result;
}