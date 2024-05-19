cmake_minimum_required(VERSION 3.25)
project(json_demo)

set(CMAKE_CXX_STANDARD 17)

include_directories("./")

file(GLOB_RECURSE SOURCES "json/json.cpp")

add_executable(json_demo ${SOURCES} main.cpp)
