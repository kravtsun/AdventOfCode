project(aoc_utils)

add_library(aoc_utils
        include/aoc_utils/io_helpers.h
        include/aoc_utils/point.h
        include/aoc_utils/field.h
        src/field.cpp)

target_include_directories(aoc_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(aoc_utils PUBLIC CMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(aoc_utils PUBLIC -fconcepts)
endif()

target_compile_features(aoc_utils PUBLIC cxx_std_20)
