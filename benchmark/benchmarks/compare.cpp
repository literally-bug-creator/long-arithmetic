#include <benchmark/benchmark.h>
#include <big_number.hpp>

using namespace big_number;

static void Is_equal(benchmark::State& state) {
    BigNumber left = make_big_number(std::string(state.range(0), '1'));
    BigNumber right = make_big_number(std::string(state.range(0), '2'));

    for (auto _ : state) {
        benchmark::DoNotOptimize(is_equal(left, right));
    }
}

static void Is_lower_than(benchmark::State& state) {
    BigNumber left = make_big_number(std::string(state.range(0), '1'));
    BigNumber right = make_big_number(std::string(state.range(0), '2'));

    for (auto _ : state) {
        benchmark::DoNotOptimize(is_lower_than(left, right));
    }
}

static void Is_greater_than(benchmark::State& state) {
    BigNumber left = make_big_number(std::string(state.range(0), '1'));
    BigNumber right = make_big_number(std::string(state.range(0), '2'));

    for (auto _ : state) {
        benchmark::DoNotOptimize(is_lower_than(left, right));
    }
}

BENCHMARK(Is_equal)->Range(1, 1 << 20);
BENCHMARK(Is_lower_than)->Range(1, 1 << 20);
BENCHMARK(Is_greater_than)->Range(1, 1 << 20);
