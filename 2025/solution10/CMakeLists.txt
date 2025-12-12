cmake_minimum_required(VERSION 3.16)
project(solution10)

set(CMAKE_CXX_STANDARD 20)

add_executable(solution10 main.cpp)

target_compile_options(solution10 PRIVATE -fconcepts)

