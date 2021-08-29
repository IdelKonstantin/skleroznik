cmake_minimum_required(VERSION 3.7)

project(test_String)

add_compile_options(-std=c++17)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/./Lib_string)

add_subdirectory(Lib_string)

add_executable(${PROJECT_NAME}

    ./test_String.cpp)

target_link_libraries(${PROJECT_NAME} String)
target_link_libraries(${PROJECT_NAME} Catch2)

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ./)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ./Lib_string)
