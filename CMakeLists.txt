cmake_minimum_required(VERSION 3.22)
project(chip8 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(thirdParty/raylib)

add_executable(chip8
    src/main.c
    src/chip8.c
    src/platform_raylib.c
)

target_link_libraries(chip8 PRIVATE raylib)