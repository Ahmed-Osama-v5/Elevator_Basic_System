cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER "gcc")

set(TARGET "elevator")

project(${TARGET})

set(SOURCES main.c)

add_executable(${TARGET} ${SOURCES})