cmake_minimum_required(VERSION 3.10)
project(sandgrain_prjct C)

set(CMAKE_C_STANDARD 17)

add_executable(sandgrain Main.c)

add_library(hourglass
        hourglass.c
        cell.c
)

target_include_directories(hourglass PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sandgrain hourglass)
