#pragma once

#include <cmath>
#include <inttypes.h>
#include <string>
#include <vector>

#include "error.hpp"

typedef __uint128_t mul_chunk;
typedef uint64_t chunk;

#define CHUNK_DIGITS 18

const chunk CHUNK_BASE = static_cast<chunk>( std::pow( 10, CHUNK_DIGITS ) );
const chunk MAX_CHUNK = CHUNK_BASE - 1;

struct BigNumber {
    std::vector<chunk> chunks;
    int32_t exponent;
    bool is_negative;
    Error error;
};

const Error& get_error( const BigNumber& number );

BigNumber make_big_number( const std::string& str );

bool is_equal( const BigNumber& left, const BigNumber& right );

bool is_lower_than( const BigNumber& left, const BigNumber& right );

BigNumber neg( const BigNumber& operand );

BigNumber add( const BigNumber& augend, const BigNumber& addend );

BigNumber sub( const BigNumber& minuend, const BigNumber& subtrahend );

BigNumber mul( const BigNumber& multiplicand, const BigNumber& multiplier );

BigNumber div( const BigNumber& dividend, const BigNumber& divisor );

BigNumber sqrt( const BigNumber& radicand );

std::string to_string( const BigNumber& number );
