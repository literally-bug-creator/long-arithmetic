#pragma once

#include <cstdint>
#include <inttypes.h>
#include <vector>

#include "error.hpp"

namespace big_number {
    typedef uint8_t digit;
    typedef std::vector<uint8_t> digits;
    typedef __uint128_t mul_chunk;
    typedef uint64_t chunk;
    typedef std::vector<chunk> chunks;

    const int32_t BASE = 18;

    const size_t MAX_DIGITS = 100000;
    const int32_t MAX_EXP = MAX_DIGITS;

    const size_t MAX_CHUNKS = MAX_DIGITS / BASE + 1;
    const size_t MIN_CHUNKS = 1;
    const int32_t MAX_SHIFT = static_cast<int32_t>( MAX_CHUNKS );

    const chunk MAX_CHUNK = 1000000000000000000;
    const chunk HALF_CHUNK = MAX_CHUNK / 2;
    const chunk ALMOST_MAX_CHUNK = MAX_CHUNK - 1;

    enum BigNumberType {
        DEFAULT,
        ZERO,
        INF,
        NAN,
    };

    struct BigNumber {
        chunks mantissa;
        int32_t shift;
        BigNumberType type;
        Error error;
        bool is_negative;
    };

    BigNumber make_big_number( const digits& digits,
                               int32_t exponent,
                               bool is_negative,
                               const Error& error );

    BigNumber make_zero( const Error& error );

    BigNumber make_nan( const Error& error );

    BigNumber make_inf( const Error& error, bool is_negative );

    const Error& get_error( const BigNumber& number );
}
