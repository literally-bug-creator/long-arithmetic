# library name: LongArithmetic
# author: Ivan Deyna, Evgeniy Sysoev, Vadim Khramtsov
# date: 2025
# description: library for long arithmetic

add_subdirectory(extern/googletest)

include(GoogleTest)

file(GLOB_RECURSE TESTS_SRC_FILES ./tests/*.cpp)
add_executable(run_tests ${TESTS_SRC_FILES})
target_link_libraries(run_tests gtest gtest_main long_arithmetic gmp gmpxx flint)
include_directories(include)

enable_testing()
include(GoogleTest)
gtest_discover_tests(run_tests)
