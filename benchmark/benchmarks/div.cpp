#include <benchmark/benchmark.h>
#include <big_number.hpp>

using namespace big_number;

static void Div(benchmark::State& state) {
    BigNumber left = make_big_number(std::string(state.range(0), '1'));
    BigNumber right = make_big_number(std::string(state.range(0), '9'));
    for (auto _ : state) {
        div(left, right);
    }
}

// static void Div_100000_Digits(benchmark::State& state) {
//     BigNumber left = make_big_number(std::string(100000, '1'));
//     BigNumber right = make_big_number(std::string(100000, '9'));
//     for (auto _ : state) {
//         auto res = div(left, right);
//         benchmark::DoNotOptimize(res);
//     }
// }

BENCHMARK(Div)->Range(1, 1 << 20);
// BENCHMARK(Div_100000_Digits);
