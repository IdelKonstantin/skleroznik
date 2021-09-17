cmake_minimum_required(VERSION 3.7)
project(myLib)

add_compile_options(-std=c++11 -g -O0 -static -Wall -Wextra)

message("Конфигурирую сборку библиотеки [${PROJECT_NAME}]")

add_library(${PROJECT_NAME} STATIC

	./src/myCoolClass.cpp
)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ./include/)

target_link_libraries(${PROJECT_NAME} pthread) #это пример добавления системной библиотеки для работы с потоками в нашу либу
