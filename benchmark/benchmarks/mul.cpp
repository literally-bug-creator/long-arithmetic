#include <benchmark/benchmark.h>
#include <big_number.hpp>

static void Mul(benchmark::State& state) {
    BigNumber left = make_big_number(std::string(state.range(0), '9'));
    BigNumber right = make_big_number(std::string(state.range(0), '9'));
    for (auto _ : state) {
        mul(left, right);
    }
}

BENCHMARK(Mul)->Range(1, 1 << 20);
