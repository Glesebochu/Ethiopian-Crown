cmake_minimum_required(VERSION 3.10)

# Project name
project(EthioCrown)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/Dependencies/include)

# Include GLM (installed in the default system include path)
include_directories(/usr/include)

# Add source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/glad.c
    ${CMAKE_SOURCE_DIR}/main.cpp
)

# Link libraries
link_directories(${CMAKE_SOURCE_DIR}/Dependencies/libraries)
set(LIBS
    glfw
    GL
    GLEW
)

# Create executable
add_executable(EthioCrown ${SOURCES})

# Link libraries to the executable
target_link_libraries(EthioCrown ${LIBS})
