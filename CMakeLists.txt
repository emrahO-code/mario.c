cmake_minimum_required(VERSION 3.10)
project(mario C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include headers
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.c")

# Find and link raylib
find_package(raylib REQUIRED)
add_executable(mario ${SOURCES})
target_link_libraries(mario raylib)

# macOS-specific frameworks
if(APPLE)
    target_link_libraries(mario
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
    )
endif()