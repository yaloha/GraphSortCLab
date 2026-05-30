cmake_minimum_required(VERSION 3.23)
project(GraphSortCLab C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_library(graph_lib STATIC
        src/graph.c
        src/fl_wrsh.c
        )
target_include_directories(graph_lib PUBLIC src)

add_executable(program main.c)
target_link_libraries(program graph_lib)

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(tests tests/tests.cpp)
    target_link_libraries(tests graph_lib gtest_main)
    enable_testing()
    add_test(NAME flwrshTest COMMAND tests)
endif()

option(BUILD_BENCHMARKS "Build unit tests" ON)
if(BUILD_BENCHMARKS)
    include(FetchContent)
    FetchContent_Declare(
            googlebenchmark
            URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.tar.gz
    )
    set(BENCHMARK_ENABLE_TESTING OFF)
    FetchContent_MakeAvailable(googlebenchmark)

    add_executable(benchmark_run benchmark/benchmark_floyd.cpp)
    target_link_libraries(benchmark_run graph_lib benchmark::benchmark_main benchmark::benchmark)
endif()
