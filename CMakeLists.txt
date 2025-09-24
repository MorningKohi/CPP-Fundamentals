cmake_minimum_required(VERSION 3.16)
project(CPP_Fundamentals VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O3)
    endif()
endif()

# Include directories
include_directories(src)
include_directories(utils)

# Find required packages
find_package(Threads REQUIRED)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9fd7188ec8e5d0a5c9e5bb7.zip
)
FetchContent_MakeAvailable(googletest)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)

# Create executables for each module
add_executable(syntax_basics src/01_fundamentals/syntax_basics.cpp)
add_executable(data_types src/01_fundamentals/data_types.cpp)  
add_executable(control_flow src/01_fundamentals/control_flow.cpp)
add_executable(functions_demo src/01_fundamentals/functions.cpp)
add_executable(classes_demo src/01_fundamentals/classes.cpp)

add_executable(stack_heap src/02_memory_management/stack_heap.cpp)
add_executable(smart_pointers src/02_memory_management/smart_pointers.cpp)
add_executable(memory_leaks src/02_memory_management/memory_leaks.cpp)
add_executable(custom_allocators src/02_memory_management/custom_allocators.cpp)

# Will add more executables as we create more modules
# add_executable(stl_containers ...)
# add_executable(algorithms ...)
# add_executable(templates ...)
# add_executable(concurrency ...)

# Enable testing
enable_testing()