cmake_minimum_required(VERSION 3.13)
project(HTTP-Server)
set(CMAKE_CXX_STANDARD 14)            

set(SOURCE_FILES src/main.cpp)

add_executable(server ${SOURCE_FILES}) 


#add_executable(client src/main-client.cpp) 

find_library(ws2_32 NAMES ws2_32)
target_link_libraries(server ws2_32)
#target_link_libraries(client ws2_32)
