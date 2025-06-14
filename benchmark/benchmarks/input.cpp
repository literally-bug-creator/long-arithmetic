#include <benchmark/benchmark.h>
#include <big_number.hpp>

#include "error.hpp"

using namespace big_number;

static void DefaultInput( benchmark::State& state ) {
    std::vector<int> digits( state.range( 0 ), 9 );

    for ( auto _ : state ) {
        BigNumber number =
            make_big_number( digits, 0, false, get_default_error() );
    }
}
BENCHMARK( DefaultInput )->Range( 1, 2 << 16 );

static void InputWithTrail( benchmark::State& state ) {
    std::vector<int> zeros( state.range( 0 ) - 1, 0 );
    std::vector<int> digits( 1, 9 );
    digits.insert( digits.end(), zeros.begin(), zeros.end() );

    for ( auto _ : state ) {
        BigNumber number =
            make_big_number( digits, 0, false, get_default_error() );
    }
}
BENCHMARK( InputWithTrail )->Range( 1, 2 << 16 );

static void InputWithLeading( benchmark::State& state ) {
    std::vector<int> digits( state.range( 0 ) - 1, 0 );
    digits.push_back( 9 );

    for ( auto _ : state ) {
        BigNumber number =
            make_big_number( digits, 0, false, get_default_error() );
    }
}
BENCHMARK( InputWithLeading )->Range( 1, 2 << 16 );
