include_directories(../inc)


ADD_LIBRARY(server
    server.cpp
)

target_include_directories(server PUBLIC ../inc)
