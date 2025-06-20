add_subdirectory(extern/googletest)

include(GoogleTest)

file(GLOB_RECURSE TESTS_SRC_FILES tests/*.cpp)
add_executable(run_tests ${TESTS_SRC_FILES})

# Find required libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED gmp)
pkg_check_modules(GMPXX REQUIRED gmpxx)

target_link_libraries(run_tests gtest gtest_main long_arithmetic ${GMP_LIBRARIES} ${GMPXX_LIBRARIES})
target_include_directories(run_tests PRIVATE ../long_arithmetic/include)
target_include_directories(run_tests PRIVATE ${GMP_INCLUDE_DIRS} ${GMPXX_INCLUDE_DIRS})

enable_testing()
gtest_discover_tests(run_tests)
