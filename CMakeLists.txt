cmake_minimum_required(VERSION 3.10)
project(OSProject C)

set(CMAKE_C_STANDARD 11)
add_executable(OSProject milestone1.c dijkstra.c dijkstra.h)
add_executable(milestone2 milestone2.c dijkstra.c dijkstra.h)
target_include_directories(milestone2 PRIVATE ${CMAKE_SOURCE_DIR}/raylib/src)
target_link_libraries(milestone2 PRIVATE ${CMAKE_SOURCE_DIR}/raylib/src/libraylib.a m pthread dl rt X11 GL)
