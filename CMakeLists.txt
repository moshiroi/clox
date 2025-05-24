cmake_minimum_required(VERSION 3.29)
project(clox C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/chunk.c
    src/memory.c
    src/debug.c
    src/value.c
)

set(HEADERS
    src/chunk.h
    src/common.h
    src/header.h
    src/memory.h
    src/debug.h
    src/value.h
)

add_executable(clox src/main.c ${SOURCES} ${HEADERS})
