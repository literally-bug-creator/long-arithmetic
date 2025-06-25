#include <benchmark/benchmark.h>
#include <big_number.hpp>

#include "constants.hpp"
#include "tools.hpp"

using namespace big_number;

static void Sub( benchmark::State& state ) {
    chunks chunks( state.range( 0 ), 999999999999999999 );
    BigNumber a = create_big_number( chunks, 9 );
    BigNumber b = create_big_number( chunks, 9 );
    for ( auto _ : state ) {
        sub( a, b );
    }
}
BENCHMARK( Sub )->Range( 1, MAX_CHUNKS );
