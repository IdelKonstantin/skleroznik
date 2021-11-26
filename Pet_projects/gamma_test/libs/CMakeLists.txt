cmake_minimum_required(VERSION 3.7)

project(test_util)

set(INC_PATH "../include")
set(SRC_PATH "../src")

file(GLOB HEADERS ${INC_PATH}/*.h)
file(GLOB SOURCES ${SRC_PATH}/*.c)

include_directories(${INC_PATH})

add_library(${PROJECT_NAME}_static STATIC ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME}_shared SHARED ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME}_static PRIVATE -Wall -std=c99 -pedantic -g -O0)
target_compile_options(${PROJECT_NAME}_shared PRIVATE -Wall -std=c99 -pedantic -g -O0)

install(TARGETS ${PROJECT_NAME}_shared DESTINATION /usr/local/lib)
