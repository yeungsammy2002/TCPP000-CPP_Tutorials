cmake_minimum_required(VERSION 3.27)
project(Reflect2)

set(CMAKE_CXX_STANDARD 17)

include_directories(./)

file(GLOB_RECURSE SOURCES "reflect/*.h" "reflect/*.cpp")

add_executable(Reflect2 ${SOURCES} main.cpp)
