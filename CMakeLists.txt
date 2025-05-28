cmake_minimum_required(VERSION 3.10)
project(mario C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include your headers
include_directories(include)

# Add raylib from submodule
add_subdirectory(extern/raylib)

# Your source files
file(GLOB SOURCES "src/*.c")

# Build the executable
add_executable(mario ${SOURCES})

# Link raylib
target_link_libraries(mario raylib)

# Optional: raylib dependencies on Linux (skip on Windows/macOS)
if(UNIX AND NOT APPLE)
    target_link_libraries(mario m pthread dl GL X11)
endif()