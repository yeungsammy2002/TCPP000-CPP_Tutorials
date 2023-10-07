cmake_minimum_required(VERSION 3.25)
project(reflect_demo)

set(CMAKE_CXX_STANDARD 17)

include_directories("./")

file(GLOB_RECURSE SOURCES "ClassFactory.cpp" "Singleton.h" "ClassRegister.h" "A.h" "B.h")

add_executable(reflect_demo ${SOURCES} main.cpp)
