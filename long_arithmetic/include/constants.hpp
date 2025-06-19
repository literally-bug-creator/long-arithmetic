#pragma once

#include <cstdint>
#include <vector>

namespace big_number {
    using digit = uint8_t;
    using digits = std::vector<digit>;
    using mul_chunk = __uint128_t;
    using chunk = uint64_t;
    using chunks = std::vector<chunk>;

    constexpr int32_t BASE = 18;
    constexpr size_t PRECISION = 100000;

    constexpr size_t MAX_CHUNKS = PRECISION / BASE + 1;
    constexpr size_t MIN_CHUNKS = 1;

    constexpr chunk MAX_CHUNK = 1000000000000000000ULL;
    constexpr chunk HALF_CHUNK = MAX_CHUNK / 2;
    constexpr chunk ALMOST_MAX_CHUNK = MAX_CHUNK - 1;

    constexpr int32_t MAX_SHIFT = []() {
        static_assert( MAX_CHUNKS <= INT32_MAX,
                       "Conversion check of 'MAX_SHIFT'" );
        return static_cast<int32_t>( MAX_CHUNKS );
    }();

    constexpr size_t MAX_DIGITS = []() {
        static_assert( MAX_CHUNKS <= SIZE_MAX / BASE,
                       "Overflow check of 'MAX_DIGITS'" );
        return MAX_CHUNKS * BASE;
    }();
    constexpr int32_t MAX_EXP = []() {
        static_assert( MAX_DIGITS <= INT32_MAX,
                       "Conversion check of 'MAX_EXP'" );
        return static_cast<int32_t>( MAX_DIGITS );
    }();
}
