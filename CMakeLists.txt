cmake_minimum_required(VERSION 3.17)
project(WaskiMost C)

set(CMAKE_C_STANDARD 11)

add_executable(WaskiMost main.c)
target_link_libraries(WaskiMost PUBLIC pthread)
