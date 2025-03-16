#cmake_minimum_required(VERSION 3.30)
#project(redis_like)
#
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
## Include directories
#include_directories(
#        include
#        ../../Boost/include
#)
#
## Gather source files
#file(GLOB SOURCES "src/*.cpp")
#
## Create the main executable
#add_executable(redis_like ${SOURCES}
#        include/command_parser.h
#        src/command_parser.cpp
#        include/catch_amalgamated.hpp
#        src/catch_amalgamated.cpp)
#
## (Optional) If you plan to add tests later, enable testing
#enable_testing()
#add_subdirectory(tests)

cmake_minimum_required(VERSION 3.15)
project(redis_like)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost
find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(include)

# Main application executable
add_executable(redis_like
        src/main.cpp
        src/database.cpp
        src/command_parser.cpp
        include/command_handler.h
        src/command_handler.cpp
        include/session.h
        src/session.cpp
        include/server.h
        src/server.cpp
)

# Link against Boost
target_link_libraries(redis_like ${Boost_LIBRARIES})

# Tests executable
add_executable(redis_tests
        tests/test.cpp
        tests/test_database.cpp
        src/database.cpp
        src/catch_amalgamated.cpp
        tests/test_concurrency.cpp
)

# Tests include directories
target_include_directories(redis_tests PRIVATE include)

# Enable testing
enable_testing()
add_test(NAME redis_tests COMMAND redis_tests)