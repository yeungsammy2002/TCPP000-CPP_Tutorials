cmake_minimum_required(VERSION 3.26)
project(xml_demo)

set(CMAKE_CXX_STANDARD 17)

include_directories("./")

file(GLOB_RECURSE SOURCES "Xml.cpp")

add_executable(xml_demo ${SOURCES} main.cpp)
