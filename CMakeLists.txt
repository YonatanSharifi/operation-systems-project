cmake_minimum_required(VERSION 4.2)
project(OSProject C)

set(CMAKE_C_STANDARD 11)

add_executable(OSProject milestone1.c
        dijkstra.c
        dijkstra.h)
