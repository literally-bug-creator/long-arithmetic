#pragma once

#include <inttypes.h>
#include <vector>

#include "error.hpp"

namespace big_number {
    typedef __uint128_t mul_chunk;
    typedef uint64_t chunk;

    const int BASE = 18;
    const chunk MAX_CHUNK = 1000000000000000000;

    struct BigNumber {
        std::vector<chunk> chunks;
        Error error;
        int32_t shift;
        bool is_negative;
    };

    BigNumber make_big_number( const std::vector<int>& digits,
                               int32_t exponent,
                               bool is_negative,
                               const Error& error );

    const Error& get_error( const BigNumber& number );
}
