#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>
#include <sstream>

using std::string;

namespace demo {
namespace fs {

class File
{
public:
    File() = default;
    File(const string & path);
    ~File() = default;

    string path() const;
    string dir() const;

    bool create();                      // create document
    bool remove();                      // remove document
    bool rename(const string & path);   // rename document
    void clear();                       // clear document
    bool exists() const;                // check if document exists
    bool copy(const string & path);     // copy document
    int line() const;                   // get number of lines
    long size() const;                  // get document size
    std::time_t time() const;           // get last modified time
    string read() const;                // read all the content of document
    bool write(const string & data);    // write all the content into document at once

private:
    string m_path;
};

}
}