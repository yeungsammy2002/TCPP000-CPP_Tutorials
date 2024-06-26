#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <climits>
#include <iostream>
#include <fs/file.h>

#ifdef WIN32
#include <dirent_win.h>
#include <direct.h>
#else
#include <dirent.h>                     // only for Linux, not for Windows
#include <sys/stat.h>
#endif

#include <utilities/string.h>

using namespace demo::utilities;

using std::string;

namespace demo {
namespace fs {

class Directory
{
public:
    Directory();
    Directory(const string & path);
    ~Directory() = default;

    string path() const;

    bool create();

    bool exists() const;                // check if directory exists

    bool remove() const;                // remove directory

    void clear();                       // clear directory

    bool rename(const string & path);   // rename directory

    bool copy(const string & path);     // copy directory

    std::vector<File> file() const;     // get all the subdirectories under the current directory

    int count() const;                  // get the total number of documents under the current directories
    int line() const;                   // get the total number of lines under the current directories
    long size() const;                  // get all the sizes of documents within the current directories

public:
    static char separator();
    static bool is_absolute_path(const string & path);
    static string normalize_path(const string & path);
    static string adjust_path(const string & path);

    static int mkdir(const char * path);
    static int rmdir(const char * path);
    static int unlink(const char * path);
    static char * getcwd(char * buf, int len);

private:
    string m_path;
};

}
}