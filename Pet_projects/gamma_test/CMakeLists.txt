cmake_minimum_required(VERSION 3.7)

project(test_util)

set(INC_PATH "include")
set(SRC_PATH "src")
set(LIB_PATH "libs")
set(LINK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/./${LIB_PATH})

message(STATUS ${LINK_PATH})

add_subdirectory(libs)

link_directories(${LINK_PATH})

add_executable(${PROJECT_NAME} ${INC_PATH} main.c)

target_link_libraries(${PROJECT_NAME} PUBLIC test_util_static)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -std=c99 -pedantic -g -O0)
