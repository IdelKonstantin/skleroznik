cmake_minimum_required(VERSION 3.7)
project(s2_server)

add_compile_options(-std=c++17 -g -O0 -static)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../BALLISTIC_SOLVER_CPP11/Libtrajsolv/)

add_executable(${PROJECT_NAME}
  server.cpp
  ./src/server_logger.cpp
  ./src/json_to_s2.cpp
  )

target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ./inc)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ../../BALLISTIC_SOLVER_CPP11/Libtrajsolv/include/)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC /usr/include/jsoncpp/)

target_link_libraries(${PROJECT_NAME} zmq)
target_link_libraries(${PROJECT_NAME} jsoncpp)
target_link_libraries(${PROJECT_NAME} trajsolv)
