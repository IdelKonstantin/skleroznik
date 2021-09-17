cmake_minimum_required(VERSION 3.7)
project(hello_world)

add_compile_options(-std=c++11 -g -O0 -static -Wall -Wextra)

add_executable(${PROJECT_NAME} 

	main.cpp
)