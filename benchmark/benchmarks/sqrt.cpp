#include <benchmark/benchmark.h>
#include <big_number.hpp>

using namespace big_number;

static void Sqrt(benchmark::State& state) {
    BigNumber value = make_big_number(std::string(state.range(0), '9'));
    for (auto _ : state) {
        sqrt(value);
    }
}

BENCHMARK(Sqrt)->Range(1, 1 << 5);
