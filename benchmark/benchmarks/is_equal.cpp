#include <benchmark/benchmark.h>
#include <big_number.hpp>

#include "constants.hpp"
#include "tools.hpp"

using namespace big_number;

static void Is_equal( benchmark::State& state ) {
    chunks lhs_chunks( state.range( 0 ), 999999999999999999 );
    chunks rhs_chunks( state.range( 0 ), 999999999999999999 );

    BigNumber lhs = create_big_number( lhs_chunks, 9 );
    BigNumber rhs = create_big_number( rhs_chunks, 9 );

    for ( auto _ : state ) {
        is_equal( lhs, rhs );
    }
}
BENCHMARK( Is_equal )->Range( 1, MAX_CHUNKS );
