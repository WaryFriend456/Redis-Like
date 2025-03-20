cmake_minimum_required(VERSION 3.15)
project(tests)

# Set C++ standard (optional, but recommended)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# List all test source files, including your production .cpp and Catch2's amalgamated .cpp
set(TEST_SOURCES
        test.cpp
        test_database.cpp
        ../src/database.cpp
        ../src/catch_amalgamated.cpp
        test_concurrency.cpp
        test_list_key_operations.cppt
)

# Create the test executable
add_executable(tests ${TEST_SOURCES})

# Ensure the compiler can find your headers (Catch2's catch_amalgamated.hpp + any project headers)
target_include_directories(tests PRIVATE
        ../include
)

# (Optional) Integrate with CTest so you can run `ctest` after building
enable_testing()
add_test(NAME runAllTests COMMAND tests)
