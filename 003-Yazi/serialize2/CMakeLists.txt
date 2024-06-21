cmake_minimum_required(VERSION 3.28)
project(serialize3)

set(CMAKE_CXX_STANDARD 17)

include_directories(./)

file(GLOB_RECURSE SOURCES "serialize/*.cpp")

add_executable(serialize3 ${SOURCES} main.cpp)

add_executable(check_endian ${SOURCES} check_endian.cpp)

add_executable(check_endian2 ${SOURCES} check_endian2.cpp)
