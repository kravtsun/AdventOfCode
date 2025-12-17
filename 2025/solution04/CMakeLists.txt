project(solution04)

add_executable(solution04 main.cpp)

target_link_libraries(solution04 aoc_utils)

target_compile_features(solution04 PUBLIC cxx_std_23)
