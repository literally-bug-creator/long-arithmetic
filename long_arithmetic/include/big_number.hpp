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

    const digit BASE = 18;
    const chunk MAX_CHUNK = 1000000000000000000;
    const int16_t MAX_INPUT_CHUNKS = 100000 / static_cast<int16_t>( BASE ) + 1;
    const int16_t MAX_CHUNKS = MAX_INPUT_CHUNKS;
    const int16_t MIN_CHUNKS = 1;
    const int32_t MAX_SHIFT = 9999;

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
                               const Error& error ) noexcept;

    BigNumber make_zero( const Error& error ) noexcept;

    BigNumber make_nan( const Error& error ) noexcept;

    BigNumber make_inf( const Error& error, bool is_negative ) noexcept;

    const Error& get_error( const BigNumber& number );
}
