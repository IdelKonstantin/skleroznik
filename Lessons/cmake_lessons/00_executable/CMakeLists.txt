cmake_minimum_required(VERSION 3.7)
project(hello_world)

add_compile_options(-std=c++11 -g -O0 -static -Wall -Wextra)

message("Конфигурирую сборку исполняемлого файла [${PROJECT_NAME}]")

add_executable(${PROJECT_NAME} 

#	./include/hello.h <- Вот так можно добавлять хедер-файлы
	./src/hello.cpp
)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ./include/) # <- но правильнее просто указать директорию с хедерами...

# Устанавливаем бинарный файл (sudo make install в консоли где Makefile)
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/bin)
