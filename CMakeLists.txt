# library name: LongArithmetic
# author: Ivan Deyna, Evgeniy Sysoev, Vadim Khramtsov
# date: 2025
# description: library for long arithmetic

cmake_minimum_required(VERSION 3.10)
project(long_arithmetic)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(long_arithmetic)

if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    message(STATUS "Building ${PROJECT_NAME} as the main project")

    option(BUILD_TESTS "Build tests" ON)
    option(BUILD_BENCHMARKS "Build benchmarks" ON)
    option(BUILD_EXAMPLE "Build example application" ON)

    if(BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
    endif()

    if(BUILD_BENCHMARKS)
        add_subdirectory(benchmark)
    endif()

    if(BUILD_EXAMPLE)
        set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g")
        set(CMAKE_CXX_FLAGS_RELEASE "-Wall -DNDEBUG")

        message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
        message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
        message(STATUS "CURRENT ENVIRONMENT: ${CMAKE_BUILD_TYPE}")

        add_executable(main main.cpp)
        target_link_libraries(main long_arithmetic)
    endif()

else()
    message(STATUS "${PROJECT_NAME} is being included as a subdirectory in another project. Skipping tests, benchmarks and examples.")
endif()
