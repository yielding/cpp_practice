cmake_minimum_required(VERSION 3.26)

project(client)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (SOURCES
    client.cpp
    main.cpp
)

include_directories (
    /usr/local/include 
    /opt/homebrew/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

link_directories (
    /usr/local/lib 
    /opt/homebrew/lib
)

add_executable(client 
    ${SOURCES}
)

#target_link_libraries(client 
## lib name 1
#    ${_LIBRARIES}
#)
