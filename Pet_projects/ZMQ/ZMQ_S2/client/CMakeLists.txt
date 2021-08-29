cmake_minimum_required(VERSION 3.7)
project(s2_client)

add_compile_options(-std=c++17 -g -O0)

add_executable(${PROJECT_NAME}
  
  client.cpp
  )

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ./inc)
target_link_libraries(${PROJECT_NAME} zmq)
target_link_libraries(${PROJECT_NAME} stdc++fs)