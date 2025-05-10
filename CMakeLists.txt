cmake_minimum_required(VERSION 3.29)
project(clox C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add_subdirectory(lib)
add_executable(clox src/main.c)
# target_link_libraries(clox PRIVATE lib)
install(TARGETS clox RUNTIME DESTINATION bin)
