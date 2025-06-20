set(BENCHMARK_USE_BUNDLED_GTEST OFF)
set(BENCHMARK_ENABLE_TESTING OFF)

add_subdirectory(extern/benchmark)


file(GLOB_RECURSE BENCHMARK_SOURCES "./benchmarks/*.cpp")
add_executable(run_benchmark ${BENCHMARK_SOURCES})

target_link_libraries(run_benchmark benchmark::benchmark long_arithmetic)
target_include_directories(run_benchmark PRIVATE ${benchmark_SOURCE_DIR}/include)
