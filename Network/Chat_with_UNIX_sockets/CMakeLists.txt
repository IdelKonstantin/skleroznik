cmake_minimum_required(VERSION 3.7)

add_compile_options(-std=c++1z)
add_subdirectory(checkSocketLib)
add_subdirectory(CUUIDGenerator)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(encryptDecrypt/Keygen)

