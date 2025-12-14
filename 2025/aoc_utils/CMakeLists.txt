project(aoc_utils)

add_library(aoc_utils INTERFACE
    include/aoc_utils/io_helpers.h
    include/aoc_utils/point.h)

target_include_directories(aoc_utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(aoc_utils INTERFACE CMAKE_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

target_compile_features(aoc_utils INTERFACE cxx_std_20)
