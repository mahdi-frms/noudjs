cmake_minimum_required(VERSION 3.18.0)
project(noudjs VERSION 0.1.0)

add_executable(noud
src/main.c
src/cwalk.c
src/evloop.c
)

add_subdirectory("dep/loop")

target_compile_options(noud PRIVATE -Werror -Wall)

target_include_directories(noud PRIVATE src)
target_link_directories(noud PRIVATE lib)
target_link_libraries(noud PRIVATE loop duktape pthread m)