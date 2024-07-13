cmake_minimum_required(VERSION 3.14)
project(copal)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(EXTRA_LIB_DIR "" CACHE PATH "Path to the extra lib directory")
set(EXTRA_INCLUDE_DIR "" CACHE PATH "Path to the extra include directory")

add_compile_options(-O3 -march=native)

include_directories(BEFORE SYSTEM "include")
include_directories(BEFORE SYSTEM ${EXTRA_INCLUDE_DIR})
include_directories(BEFORE SYSTEM ${CMAKE_CURRENT_LIST_DIR}/include)

set(GTest_DIR ${EXTRA_LIB_DIR}/cmake/GTest)
find_package(GTest REQUIRED CONFIG)

set(benchmark_DIR ${EXTRA_LIB_DIR}/cmake/benchmark)
find_package(benchmark REQUIRED CONFIG)

file(GLOB_RECURSE COPAL_CPP ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
add_library(copal ${COPAL_CPP})

file(GLOB_RECURSE BENCHMARK_CPP ${CMAKE_CURRENT_LIST_DIR}/benchmark/*.cpp)
add_executable(benchmark ${BENCHMARK_CPP})
target_link_libraries(benchmark PRIVATE copal benchmark::benchmark profiler)

file(GLOB_RECURSE TEST_CPP ${CMAKE_CURRENT_LIST_DIR}/*.test.cc)
add_executable(test ${TEST_CPP})
target_include_directories(test PRIVATE "test")
target_link_libraries(test PRIVATE copal GTest::gtest GTest::gtest_main)
