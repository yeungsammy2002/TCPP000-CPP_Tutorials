cmake_minimum_required(VERSION 3.26)
project(ini_demo)

set(CMAKE_CXX_STANDARD 17)

include_directories("./")

add_executable(ini_demo main.cpp
        IniFile.h
        IniFile.cpp)
