cmake_minimum_required(VERSION 3.14)
project(Lab1)

set(CMAKE_CXX_STANDARD 17)

add_library(graph_lib STATIC test.cpp)

option(ENABLE_COVERAGE "Enable coverage flags" OFF)
if(ENABLE_COVERAGE)
    target_compile_options(graph_lib PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    target_link_options(graph_lib PRIVATE -fprofile-instr-generate -fcoverage-mapping)
endif()

find_package(GTest REQUIRED)

add_executable(graph_test tests.cpp)
target_link_libraries(graph_test PRIVATE graph_lib GTest::gtest_main)

enable_testing()
add_test(NAME GraphTest COMMAND graph_test)