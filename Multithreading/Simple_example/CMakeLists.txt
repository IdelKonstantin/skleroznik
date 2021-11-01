cmake_minimum_required(VERSION 3.7)

#set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++1z -ggdb3 -O0)

add_executable(server server.cpp)
target_link_libraries(server zmq)

add_executable(client client.cpp)
target_link_libraries(client zmq pthread)