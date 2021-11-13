cmake_minimum_required(VERSION 3.7)
project(keyGen)

add_compile_options(-std=c++17 -g -O0)

add_executable(${PROJECT_NAME} 
	
	keyGen.cpp
)

target_link_libraries(${PROJECT_NAME} crypto++)
target_link_libraries(${PROJECT_NAME} stdc++fs)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC /usr/include/crypto++/)

