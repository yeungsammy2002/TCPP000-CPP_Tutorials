#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <iostream>
#include <dirent.h>

#include <unistd.h>

#include <utilities/string.h>
#include <utilities/file.h>

using std::string;

class Directory
{
public:
    Directory();
    Directory(const string & path);
    ~Directory() = default;

    string path() const;
    bool create();
    bool exists() const;
    bool remove() const;
    void clear();
    bool rename(const string & path);
    bool copy(const string & path);
    std::vector<File> file() const;     // get all the docs under current directory

    static char separator();
    static bool is_absolute_path(const string & path);
    static string normalize_path(const string & path);
    static string adjust_path(const string & path);

private:
    string m_path;
};
