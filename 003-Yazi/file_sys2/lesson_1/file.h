#pragma once

#include <string>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <sys/stat.h>

using std::string;

class File
{
public:
    File() = delete;
    File(const string& path);
    ~File() = default;

    string path() const;
    string dir() const;
    bool create();
    bool remove();
    bool rename(const string& path);
    void clear();
    bool exists() const;
    bool copy(const string& path);
    int line() const;
    long size() const;

private:
    string m_path;
};