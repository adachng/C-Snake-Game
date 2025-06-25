cmake_minimum_required(VERSION 3.22)
project(C-Snake-Game
    HOMEPAGE_URL "https://github.com/adachng/C-Snake-Game"
    LANGUAGES C
)

# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>") # for all executables
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>") # for libSDL3.so etc.

add_subdirectory(third_party)
add_subdirectory(spike)
add_subdirectory(src)
add_subdirectory(test)
