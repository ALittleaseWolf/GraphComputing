# cmake版本
cmake_minimum_required(VERSION 3.2)

# project项目
PROJECT(test)
set(CMAKE_CXX_STANDARD 11)
add_definitions("-D DEBUG -D TEST")
# headfile
INCLUDE_DIRECTORIES(
    ../universal
)

# set environment variable
SET(TEST_MATH bfs.cpp)

# add executable file
ADD_EXECUTABLE(bfs bfs.cpp)

