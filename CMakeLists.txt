cmake_minimum_required(VERSION 3.14)
project(goomy)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} src/goomy.cpp include/goomy/goomy.h)
target_include_directories(${PROJECT_NAME} PUBLIC include)

add_subdirectory(examples/basic)
