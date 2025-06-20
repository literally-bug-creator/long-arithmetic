# library name: LongArithmetic
# author: Ivan Deyna, Evgeniy Sysoev, Vadim Khramtsov
# date: 2025
# description: library for long arithmetic

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Собираем APA как отдельную библиотеку
file(GLOB APA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/APA/*.cpp")
add_library(apa STATIC ${APA_SOURCES})
target_include_directories(apa PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/APA")

# Собираем вашу библиотеку только из source
file(GLOB_RECURSE SOURCES "source/*.cpp")
add_library(long_arithmetic SHARED ${SOURCES})
target_include_directories(long_arithmetic PUBLIC include "${CMAKE_CURRENT_SOURCE_DIR}")

# Линкуем с APA
target_link_libraries(long_arithmetic PUBLIC apa)
