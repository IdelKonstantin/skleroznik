cmake_minimum_required(VERSION 3.7)
project(client)

add_compile_options(-std=c++17 -g -O0)

link_directories(${CMAKE_CURRENT_SOURCE_DIR} ../checkSocketLib)

add_executable(${PROJECT_NAME} 
	
	clientWorker.cpp
	chatClient.cpp
)

target_link_libraries(${PROJECT_NAME} jsoncpp)
target_link_libraries(${PROJECT_NAME} commonStuff)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ../Include/)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC /usr/include/jsoncpp/)

