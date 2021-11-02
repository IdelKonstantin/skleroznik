cmake_minimum_required(VERSION 3.7)
project(commonStuff)

add_compile_options(-std=c++17 -g -O0)

add_library(${PROJECT_NAME} 
	
	commonStuff
)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ../Include/)

