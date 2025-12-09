project(solution05)

add_executable(solution05 main.cpp)

target_link_libraries(solution05 aoc_utils)

target_compile_features(solution05 PUBLIC cxx_std_23)
