#include <benchmark/benchmark.h>
#include <big_number.hpp>

static void CreateFromString(benchmark::State& state) {
    std::string str(state.range(0), '1');
    for (auto _ : state) {
        BigNumber number = make_big_number(str);
    }
}
BENCHMARK(CreateFromString)->Range(1, 1 << 20);
