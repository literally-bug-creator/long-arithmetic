add_subdirectory(extern/googletest)

include(GoogleTest)

file(GLOB_RECURSE TESTS_SRC_FILES ./tests/*.cpp)
add_executable(run_tests ${TESTS_SRC_FILES})
target_link_libraries(run_tests gtest gtest_main long_arithmetic gmp gmpxx)
include_directories(include)

enable_testing()
include(GoogleTest)
gtest_discover_tests(run_tests)
