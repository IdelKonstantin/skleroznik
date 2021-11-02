cmake_minimum_required(VERSION 3.7)
project(uuidManager)

add_compile_options(-std=c++1z -g)

find_library(SSL_LIB ssl HINTS /usr/lib/x86_64-linux-gnu)

if(${SSL_LIB} STREQUAL "SSL_LIB-NOTFOUND")
    message(FATAL_ERROR "Library libssl.so not found. Install it by using command: sudo apt-get install libssl-dev")
endif()

add_library(uuidManager CUUIDGenerator.cpp)

target_include_directories(uuidManager SYSTEM PUBLIC ../Include)

#target_link_libraries(crypto)
