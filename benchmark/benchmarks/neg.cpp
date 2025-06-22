#include <benchmark/benchmark.h>
#include <big_number.hpp>

#include "constants.hpp"
#include "tools.hpp"

using namespace big_number;

static void Neg( benchmark::State& state ) {
    chunks chunks( state.range( 0 ), 999999999999999999 );
    BigNumber number = create_big_number( chunks, 9 );
    for ( auto _ : state ) {
        neg( number );
    }
}
BENCHMARK( Neg )->Range( 1, MAX_CHUNKS );
