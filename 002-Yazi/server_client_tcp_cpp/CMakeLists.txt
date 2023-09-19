cmake_minimum_required(VERSION 3.26)
project(server_cpp_demo)

set(CMAKE_CXX_STANDARD 17)

include_directories("./")

file(GLOB_RECURSE SOURCES "utilities/*.cpp" "socket/*.cpp")

add_executable(server ${SOURCES} server.cpp)
add_executable(client ${SOURCES} client.cpp)
