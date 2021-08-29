cmake_minimum_required(VERSION 3.7)
project(JSON_EXTRACTOR)

add_compile_options(-std=c++17 -g -O0 -static)

add_executable(JSON_EXTRACTOR 

	Amunition_extractor_var2.cpp 
	./src/ammo_extractor_saver.cpp
	./src/sqlite3.c
)

target_link_libraries(${PROJECT_NAME} jsoncpp)
target_link_libraries(${PROJECT_NAME} dl)
target_link_libraries(${PROJECT_NAME} pthread)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ./inc/)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC /usr/include/jsoncpp/)

