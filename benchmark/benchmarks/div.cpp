#include <benchmark/benchmark.h>
#include <big_number.hpp>

static void Div(benchmark::State& state) {
    BigNumber left = make_big_number(std::string(state.range(0), '1'));
    BigNumber right = make_big_number(std::string(state.range(0), '9'));
    for (auto _ : state) {
        div(left, right);
    }
}

BENCHMARK(Div)->Range(1, 1 << 20);
