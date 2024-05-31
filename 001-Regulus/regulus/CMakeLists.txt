cmake_minimum_required(VERSION 3.28)
project(regulus)

set(CMAKE_CXX_STANDARD 17)

include_directories(./)

file(GLOB_RECURSE SOURCES
        "parsers/json/*.h"
        "parsers/json/*.cpp"
        "utilities/cmd_opts/*.h"
        "utilities/cmd_opts/*.cpp"
        "utilities/err/*.h"
        "utilities/err/*.cpp"
        "utilities/str/*.h"
        "utilities/str/*.cpp"
)

add_executable(regulus ${SOURCES} main.cpp)
