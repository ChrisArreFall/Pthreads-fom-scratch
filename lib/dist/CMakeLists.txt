cmake_minimum_required(VERSION 3.15)
project(dist C)

set(CMAKE_C_STANDARD 11)

add_executable(dist main.c dist/dist.h)
target_link_libraries(dist m)