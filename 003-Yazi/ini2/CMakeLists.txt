cmake_minimum_required(VERSION 3.28)
project(ini3)

set(CMAKE_CXX_STANDARD 17)

include_directories(./)

file(GLOB_RECURSE SOURCES "utilities/*.cpp")

add_executable(ini3 ${SOURCES} main.cpp)
