cmake_minimum_required(VERSION 3.16)
project(solution09)

set(CMAKE_CXX_STANDARD 20)

add_executable(solution09 main.cpp)

target_compile_options(solution09 PRIVATE -fconcepts)

