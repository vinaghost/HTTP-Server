set(CMAKE_CXX_STANDARD 14)

include_directories(../inc)
find_library(ws2_32 NAMES ws2_32)

add_library(server
    server.cpp
)

target_include_directories(server PUBLIC ../inc)
target_link_libraries(server ws2_32)

add_library(header 
    header.cpp
    request.cpp
    response.cpp
)

target_include_directories(header PUBLIC ../inc)
