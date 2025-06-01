# library name: LongArithmetic
# author: Ivan Deyna, Evgeniy Sysoev, Vadim Khramtsov
# date: 2025
# description: library for long arithmetic

file (GLOB_RECURSE SOURCES "src/*.cpp")
add_library(long_arithmetic SHARED ${SOURCES})

target_include_directories(long_arithmetic PUBLIC include)
