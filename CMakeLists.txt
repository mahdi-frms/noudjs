cmake_minimum_required(VERSION 3.18.0)
project(godeljs VERSION 0.1.0)

add_executable(godel
src/main.c
src/cwalk.c
)

add_subdirectory("dep/loop")

SET(GCC_COVERAGE_COMPILE_FLAGS " -g -Werror -Wall")

target_include_directories(godel PRIVATE src)
target_link_directories(godel PRIVATE lib)
target_link_libraries(godel PRIVATE loop duktape pthread m)