#include <benchmark/benchmark.h>
#include <big_number.hpp>

#include "error.hpp"

using namespace big_number;

static void Input( benchmark::State& state ) {
    std::vector<int> digits( state.range( 0 ), 9 );

    for ( auto _ : state ) {
        BigNumber number =
            make_big_number( digits, 0, false, get_default_error() );
    }
}
BENCHMARK( Input )->Range( 1, 2 << 16 );
