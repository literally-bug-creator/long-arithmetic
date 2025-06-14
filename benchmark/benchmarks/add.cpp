#include <benchmark/benchmark.h>
#include <big_number.hpp>

using namespace big_number;

static void Add( benchmark::State& state ) {
    std::vector<int> a_digits( state.range( 0 ), 9 );
    std::vector<int> b_digits( state.range( 0 ), 9 );
    BigNumber number1 = make_big_number( a_digits );
    BigNumber number2 = make_big_number( b_digits );

    for ( auto _ : state ) {
        add( number1, number2 );
    }
}
BENCHMARK( Add )->Range( 1, 2 << 16 );
