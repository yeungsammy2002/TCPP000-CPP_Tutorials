#pragma once

#include <string>
#include <stdarg.h>
#include <stdexcept>
#include <sstream>

using std::string;

namespace regulus {
namespace utilities {

class Err
{
public:
    static void logic(const char * format, ...);
};

}
}