cmake_minimum_required(VERSION 3.13)
project(HTTP-Server)
set(CMAKE_CXX_STANDARD 14)            

set(SOURCE_FILES src/main.cpp)

add_executable(main ${SOURCE_FILES}) 

find_library(ws2_32 NAMES ws2_32)
add_subdirectory(src)
target_link_libraries(main server ws2_32)
