cmake_minimum_required(VERSION 3.30)
project(tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB TEST_SOURCES "*.cpp")
add_executable(tests ${TEST_SOURCES})