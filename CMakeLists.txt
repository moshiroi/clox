cmake_minimum_required(VERSION 3.29)
project(clox C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    src/chunk.c
    src/memory.c
    src/debug.c
    src/value.c
    src/vm.c
)

set(HEADERS
    src/chunk.h
    src/common.h
    src/header.h
    src/memory.h
    src/debug.h
    src/value.h
    src/vm.h
)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_executable(clox src/main.c ${SOURCES} ${HEADERS})
