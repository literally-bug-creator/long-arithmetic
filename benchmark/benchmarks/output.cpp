#include <benchmark/benchmark.h>
#include <big_number.hpp>

#include "constants.hpp"
#include "tools.hpp"

using namespace big_number;

static void Output( benchmark::State& state ) {
    chunks chunks( state.range( 0 ), 999999999999999999 );
    BigNumber number = create_big_number( chunks, 9 );
    for ( auto _ : state ) {
        to_string( number );
    }
}
BENCHMARK( Output )->Range( 1, MAX_CHUNKS );
