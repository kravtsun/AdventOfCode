cmake_minimum_required(VERSION 3.16)
project(solution11)

set(CMAKE_CXX_STANDARD 20)

add_executable(solution11 main.cpp)

target_compile_options(solution11 PRIVATE -fconcepts)

