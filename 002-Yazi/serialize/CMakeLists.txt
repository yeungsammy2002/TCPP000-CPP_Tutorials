cmake_minimum_required(VERSION 3.26)
project(serialize_demo)

set(CMAKE_CXX_STANDARD 17)

include_directories("./")

file(GLOB_RECURSE SOURCES "DataStream.cpp")

add_executable(serialize_demo ${SOURCES} main.cpp
        serialize/DataStream.h
        serialize/DataSream.cpp
)
