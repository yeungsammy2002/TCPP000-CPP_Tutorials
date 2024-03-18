cmake_minimum_required(VERSION 3.27)
project(Vector2)

set(CMAKE_CXX_STANDARD 17)

include_directories("./")

file(GLOB_RECURSE SOURCES "stl/*.h" "stl/*.inl")

add_executable(Vector2 ${SOURCES} main.cpp)

add_executable(speedtest ${SOURCES} speedtest.cpp)

add_executable(speedtest2 ${SOURCES} speedtest2.cpp)