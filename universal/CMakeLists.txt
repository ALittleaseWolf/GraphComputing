# cmake版本
cmake_minimum_required(VERSION 3.2)

# project项目
PROJECT(convert)
set(CMAKE_CXX_STANDARD 11)
add_definitions("-D DEBUG -D TEST -D CONVERT")
# headfile
INCLUDE_DIRECTORIES(
    .
)

# set environment variable
SET(TEST_MATH convert.cpp)

# add executable file
ADD_EXECUTABLE(convert convert.cpp)

