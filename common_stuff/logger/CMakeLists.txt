cmake_minimum_required(VERSION 3.7)
project(logger)

add_compile_options(-std=c++1z -g -O0)

add_library(${PROJECT_NAME} STATIC 
	
	CFastLog.cpp	
)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ./)
