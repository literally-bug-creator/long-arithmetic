#pragma once

#include <cmath>
#include <inttypes.h>
#include <vector>

#include "error.hpp"

namespace big_number {
    typedef __uint128_t mul_chunk;
    typedef uint64_t chunk;

    const int BASE = 18;
    const chunk MAX_CHUNK = static_cast<chunk>( std::pow( 10, BASE ) );
    const int32_t ONE_INT = 1;
    const int32_t ZERO_INT = 0;

    struct BigNumber {
        std::vector<chunk> chunks;
        int32_t exponent;
        bool is_negative;
        Error error;
    };

    BigNumber make_big_number( std::vector<int> digits = {},
                               int32_t exponent = 0,
                               bool is_negative = false,
                               const Error& error = get_default_error() );

    const Error& get_error( const BigNumber& number );

    bool is_equal( const BigNumber& left, const BigNumber& right );

    bool is_lower_than( const BigNumber& left, const BigNumber& right );

    bool is_greater_than( const BigNumber& left, const BigNumber& right );

    BigNumber abs( const BigNumber& operand );

    BigNumber neg( const BigNumber& operand );

    BigNumber add( const BigNumber& augend, const BigNumber& addend );

    BigNumber sub( const BigNumber& minuend, const BigNumber& subtrahend );

    BigNumber mul( const BigNumber& multiplicand, const BigNumber& multiplier );

    std::string to_string( const BigNumber& number );
}
