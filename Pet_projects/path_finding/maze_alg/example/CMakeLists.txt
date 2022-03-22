cmake_minimum_required(VERSION 3.7)

project(maze)

include_directories("../include")

link_directories("../build")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC maze_alg)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -std=c++1z -g -O0)
