cmake_minimum_required(VERSION 3.16)
project(AdventOfCode)

set(CMAKE_CXX_STANDARD 17)

#add_executable(AdventOfCode main.cpp)

foreach (i RANGE 1 25)
    if (EXISTS "${PROJECT_SOURCE_DIR}/solution${i}/main.cpp")
        add_executable(solution${i} "${PROJECT_SOURCE_DIR}/solution${i}/main.cpp")
        target_compile_definitions(solution${i} PRIVATE WORKDIR="${PROJECT_SOURCE_DIR}/solution${i}/")
    endif ()
endforeach ()

add_executable(solution5_1 solution5/main1.cpp)
target_compile_definitions(solution5_1 PRIVATE WORKDIR="${PROJECT_SOURCE_DIR}/solution5/")
