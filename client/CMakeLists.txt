cmake_minimum_required(VERSION 3.26)
project(client C)

set(CMAKE_C_STANDARD 11)

add_executable(client clientWIN.c)


if (WIN32)
    target_link_libraries(client ws2_32)
endif()
