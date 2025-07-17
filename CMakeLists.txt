cmake_minimum_required(VERSION 3.10)
project(mario_game C)

file(GLOB sources "src/*.c")

set(raylib_DIR "/usr/local/lib/cmake/raylib")
set(raylib_LIBRARY "/usr/local/lib/libraylib.so")
# Find raylib package
find_package(raylib REQUIRED)

# Create executable named 'mario' from source file 'mario.c'
add_executable(mario ${sources})

# Link raylib library
target_link_libraries(mario PRIVATE raylib)

# Set C standard to C11 (optional)
set_target_properties(mario PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
)
target_include_directories(mario PRIVATE include)

# On Linux, link pthread, dl, and math libraries explicitly
if(UNIX AND NOT APPLE)
    target_link_libraries(mario PRIVATE pthread dl m)
endif()
