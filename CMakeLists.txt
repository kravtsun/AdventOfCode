cmake_minimum_required(VERSION 3.16)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 17)

#add_executable(AdventOfCode main.cpp)

foreach (i RANGE 1 8)
    add_executable(solution${i} "solution${i}/main.cpp")
    target_compile_definitions(solution${i} PRIVATE WORKDIR="${PROJECT_SOURCE_DIR}/solution${i}/")
endforeach ()
