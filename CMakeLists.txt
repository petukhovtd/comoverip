cmake_minimum_required(VERSION 3.16)
project(comoverip)

set(CMAKE_CXX_STANDARD 20)

add_executable(comoverip main.cpp)

add_subdirectory(contrib/googletest)
add_subdirectory(test)