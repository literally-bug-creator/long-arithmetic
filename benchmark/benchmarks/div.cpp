#include <benchmark/benchmark.h>
#include <big_number.hpp>

static void Div(benchmark::State& state) {
    BigNumber left = make_big_number("1");
    BigNumber right = make_big_number("9");
    for (auto _ : state) {
        div(left, right);
    }
}

BENCHMARK(Div)->Range(1, 1 << 20);
