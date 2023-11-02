cmake_minimum_required(VERSION 3.26)
project(command_line_demo)

set(CMAKE_CXX_STANDARD 17)

include_directories("./")

file(GLOB_RECURSE SOURCES "utilities/option.cpp")

add_executable(command_line_demo ${SOURCES} main.cpp)
