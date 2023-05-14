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

# googletest 라이브러리 가져오기
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# 테스트 타겟 추가
add_executable(pump_packet pump_packet.cpp)
target_link_libraries(pump_packet pump_packet_main)
