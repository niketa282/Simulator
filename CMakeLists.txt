cmake_minimum_required (VERSION 3.14)
project (Simulator VERSION 0.1 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library (Simulator STATIC programCounter.cpp)
target_include_directories (Simulator PUBLIC inc/)

add_subdirectory(googletest-release-1.12.1)

add_executable (test-programCounter test-programCounter.cpp)
target_link_libraries (test-programCounter PRIVATE Simulator gtest gtest_main)