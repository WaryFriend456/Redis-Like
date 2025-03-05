cmake_minimum_required(VERSION 3.30)
project(redis_like)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_executable(redis_like src/main.cpp
#        include/database.h
#        src/database.cpp
#        tests/test_database.cpp)

# Include directories
include_directories(include)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Create the main executable
add_executable(redis_like ${SOURCES})

# (Optional) If you plan to add tests later, enable testing
enable_testing()
add_subdirectory(tests)