#include <benchmark/benchmark.h>
#include <big_number.hpp>

using namespace big_number;

static void FromString(benchmark::State& state) {
    std::string str(state.range(0), '1');
    for (auto _ : state) {
        BigNumber number = make_big_number(str);
    }
}
BENCHMARK(FromString)->Range(1, 1 << 20);

// static void FromString_100000_Digits(benchmark::State& state) {
//     std::string str(100000, '1');
//     for (auto _ : state) {
//         BigNumber number = make_big_number(str);
//         // benchmark::DoNotOptimize(number);
//     }
// }

// BENCHMARK(FromString_100000_Digits);
