#pragma once

#include <iostream>
#include <string>

using std::string;

namespace regulus {
namespace utilities {

class Log
{
public:
    enum Type
    {
        CONSTRUCT = 0,
        CONSTRUCT_BOOL,
        CONSTRUCT_INT,
        CONSTRUCT_DOUBLE,
        CONSTRUCT_STRING,
        CPY_CONSTRUCT,
        CPY_ASSIGN,
        MV_CONSTRUCT,
        MV_ASSIGN,
        DESTRUCT
    };
    
    static void log(const string & msg, ...);
private:
};

}
}