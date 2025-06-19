#pragma once

#include <cstdint>
#include <inttypes.h>
#include <vector>

#include "error.hpp"

namespace big_number {
    using digit = uint8_t;
    using digits = std::vector<digit>;
    using mul_chunk = __uint128_t;
    using chunk = uint64_t;
    using chunks = std::vector<chunk>;

    const int32_t BASE = 18;
    const size_t PRECISION = 100000;

    const chunk MAX_CHUNK = 1000000000000000000;
    const chunk HALF_CHUNK = MAX_CHUNK / 2;
    const chunk ALMOST_MAX_CHUNK = MAX_CHUNK - 1;

    const size_t MAX_CHUNKS = PRECISION / BASE + 1;
    const size_t MIN_CHUNKS = 1;
    const int32_t MAX_SHIFT = static_cast<int32_t>( MAX_CHUNKS );

    const size_t MAX_DIGITS = MAX_CHUNKS * BASE;
    const int32_t MAX_EXP = MAX_DIGITS;

    enum BigNumberType {
        DEFAULT,
        ZERO,
        INF,
        NOT_A_NUMBER,
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
