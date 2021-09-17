cmake_minimum_required(VERSION 3.7)
project(exec_with_linking)

add_compile_options(-std=c++11 -g -O0 -Wall -Wextra)

message("Конфигурирую сборку исполняемлого файла [${PROJECT_NAME}]")

link_directories(${CMAKE_CURRENT_SOURCE_DIR} ../01_static_lib)

add_executable(${PROJECT_NAME} 

	main.cpp
)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ../01_static_lib/include/)
target_link_libraries(${PROJECT_NAME} myLib)
