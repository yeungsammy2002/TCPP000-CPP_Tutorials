cmake_minimum_required(VERSION 3.28)
project(Json3)

set(CMAKE_CXX_STANDARD 17)

include_directories(./)

file(GLOB_RECURSE SOURCES "json/*.cpp")

add_executable(Json3 ${SOURCES} main.cpp)
