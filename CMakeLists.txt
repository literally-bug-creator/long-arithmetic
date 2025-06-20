cmake_minimum_required(VERSION 3.16)
project(long_arithmetic VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(long_arithmetic)

if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    message(STATUS "Building ${PROJECT_NAME} as the main project")

    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -O3 -DNDEBUG -march=native -flto -funroll-loops")

    add_subdirectory(tests)
    add_subdirectory(benchmark)

    message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
    message(STATUS "CURRENT ENVIRONMENT: ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "${PROJECT_NAME} is being included as a module")
endif()
