cmake_minimum_required(VERSION 3.13)
project(HTTP-Server)

set(SOURCE_FILES src/main.cpp)

add_executable(main ${SOURCE_FILES}) 

add_subdirectory(src)
target_link_libraries(main server header)
