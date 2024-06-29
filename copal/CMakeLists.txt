cmake_minimum_required(VERSION 3.14)
project(Project1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTRA_LIB_DIR "" CACHE PATH "Path to the extra lib directory")
set(EXTRA_INCLUDE_DIR "" CACHE PATH "Path to the extra include directory")

add_compile_options(-O3 -march=native)

link_directories(${EXTRA_LIB_DIR})
include_directories(${EXTRA_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

file(GLOB_RECURSE BENCHMARK_CPP ${CMAKE_CURRENT_LIST_DIR}/benchmark/*.cpp)
add_executable(benchmark ${BENCHMARK_CPP})
target_include_directories(benchmark PRIVATE "includes")
target_link_libraries(benchmark benchmark::benchmark stdc++ m)

file(GLOB_RECURSE TEST_CPP ${CMAKE_CURRENT_LIST_DIR}/*.test.cc)
add_executable(test ${TEST_CPP})
target_include_directories(test PRIVATE "includes")
target_include_directories(test PRIVATE "test")
target_link_libraries(test GTest::gtest GTest::gtest_main stdc++ m)
