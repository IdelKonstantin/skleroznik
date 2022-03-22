cmake_minimum_required(VERSION 3.7)

project(island)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -std=c++1z -g -O0)
