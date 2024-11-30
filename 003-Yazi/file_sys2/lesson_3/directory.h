#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <unistd.h>
//#include <dirent.h>

#include <utilities/string.h>

using std::string;

class Directory
{
public:
    Directory();
    Directory(const string& path);
    ~Directory() = default;

    string path() const;

    static char separator();
    static bool is_absolute_path(const string& path);
    static string normalize_path(const string& path);
    static string adjust_path(const string& path);

private:
    string m_path;
};