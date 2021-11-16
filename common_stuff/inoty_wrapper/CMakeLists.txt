cmake_minimum_required(VERSION 3.7)

project(inoty_serv)

add_compile_options(-std=c++1z -g)

add_library(${PROJECT_NAME} 
	inoty_wrapper.h
	inoty_wrapper.cpp	
)