cmake_minimum_required(VERSION 3.18)
project(pubg_suite)

# C++20 Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -fvisibility=hidden \
    -fvisibility-inlines-hidden \
    -O2 \
    -Wno-unused-variable \
    -Wno-unused-function \
    -Wno-write-strings \
    -fPIC \
    -shared \
    -Wl,--build-id=none \
    -Wl,-z,now \
    -Wl,-z,relro \
    -fdata-sections \
    -ffunction-sections")

# Android-specific settings
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find required libraries
find_library(log-lib log)
find_library(egl-lib EGL)
find_library(gles-lib GLESv3)
find_library(android-lib android)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
)

# Source files
add_library(pubg_suite SHARED main.cpp)

# Link libraries
target_link_libraries(pubg_suite 
    ${log-lib} 
    ${egl-lib} 
    ${gles-lib} 
    ${android-lib}
)

# Output directory
set_target_properties(pubg_suite PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_NAME "pubg_suite"
)