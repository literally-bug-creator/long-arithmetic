#include <benchmark/benchmark.h>
#include <big_number.hpp>

static void Add(benchmark::State& state) {
    BigNumber left = make_big_number(std::string(state.range(0), '9'));
    BigNumber right = make_big_number(std::string(state.range(0), '9'));
    for (auto _ : state) {
        add(left, right);
    }
}

BENCHMARK(Add)->Range(1, 1 << 20);
