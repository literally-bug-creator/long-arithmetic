#include <benchmark/benchmark.h>
#include <big_number.hpp>

using namespace big_number;

static void Add(benchmark::State& state) {
    BigNumber left = make_big_number(std::string(state.range(0), '9'));
    BigNumber right = make_big_number(std::string(state.range(0), '9'));
    for (auto _ : state) {
        add(left, right);
    }
}

// static void Add_100000_Digits(benchmark::State& state) {
//     BigNumber left = make_big_number(std::string(100000, '9'));
//     BigNumber right = make_big_number(std::string(100000, '9'));
//     for (auto _ : state) {
//         auto res = add(left, right);
//         // benchmark::DoNotOptimize(res);
//     }
// }

BENCHMARK(Add)->Range(1, 1 << 20);
// BENCHMARK(Add_100000_Digits);
