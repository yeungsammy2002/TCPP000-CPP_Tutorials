#pragma once

#include <string>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <sstream>

using std::string;

class File
{
public:
    File() = delete;
    File(const string & path);
    ~File() = default;

    string path() const;
    string dir() const;

    bool create();
    bool remove();
    bool rename(const string & path);
    void clear();
    bool exists();
    bool copy(const string & path);
    int line() const;
    long size() const;
    time_t time() const;
    string read() const;
    bool write(const string & file);

private:
    string m_path;
};