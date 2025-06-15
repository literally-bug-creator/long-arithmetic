#include <benchmark/benchmark.h>
#include <big_number.hpp>

using namespace big_number;

static void DefaultInput( benchmark::State& state ) {
    std::vector<uint8_t> digits( state.range( 0 ), 9 );

    for ( auto _ : state ) {
        BigNumber number =
            make_big_number( digits, 0, false, get_default_error() );
    }
}
BENCHMARK( DefaultInput )->Range( 1, 100000 );
