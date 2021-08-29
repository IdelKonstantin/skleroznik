cmake_minimum_required(VERSION 3.7)

project(String)

add_compile_options(-std=c++11)

add_library(${PROJECT_NAME}
    ./String.cpp)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ./)
