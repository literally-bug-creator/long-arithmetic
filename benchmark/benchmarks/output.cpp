#include <benchmark/benchmark.h>
#include <big_number.hpp>

#include "constants.hpp"

using namespace big_number;

BigNumber create_big_number(
    const chunks& chunks,
    int32_t shift,
    bool is_negative = false,
    big_number::BigNumberType type = big_number::BigNumberType::DEFAULT ) {
    big_number::BigNumber number;
    number.mantissa = chunks;
    number.shift = shift;
    number.is_negative = is_negative;
    number.type = type;
    number.error = big_number::Error{};
    return number;
}

static void Output( benchmark::State& state ) {
    chunks chunks( state.range( 0 ), 999999999999999999 );
    BigNumber number = create_big_number( chunks, 9 );
    for ( auto _ : state ) {
        to_string( number );
    }
}
BENCHMARK( Output )->Range( 1, MAX_CHUNKS );
