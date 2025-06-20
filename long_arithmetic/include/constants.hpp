#pragma once

#include <cstdint>
#include <limits>
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

    constexpr int32_t MAX_SHIFT = static_cast<int32_t>( MAX_CHUNKS );

    constexpr size_t MAX_DIGITS = MAX_CHUNKS * BASE;
    constexpr int32_t MAX_EXP = static_cast<int32_t>( MAX_DIGITS );

    static_assert( BASE > 0, "BASE must be positive" );
    static_assert( PRECISION > 0, "PRECISION must be positive" );
    static_assert( MAX_CHUNKS > MIN_CHUNKS, "MAX_CHUNKS must be > MIN_CHUNKS" );

    static_assert( MAX_CHUNKS <= SIZE_MAX / BASE,
                   "MAX_DIGITS calculation would overflow" );
    static_assert( MAX_CHUNKS <= std::numeric_limits<int32_t>::max(),
                   "MAX_SHIFT conversion unsafe" );
    static_assert( MAX_DIGITS <= std::numeric_limits<int32_t>::max(),
                   "MAX_EXP conversion unsafe" );

    static_assert( MAX_CHUNK > HALF_CHUNK, "MAX_CHUNK sanity check failed" );
    static_assert( ALMOST_MAX_CHUNK == MAX_CHUNK - 1,
                   "ALMOST_MAX_CHUNK calculation incorrect" );

    static_assert( std::is_trivially_copyable_v<chunk>,
                   "chunk must be trivially copyable" );
    static_assert( std::is_unsigned_v<chunk>, "chunk must be unsigned" );
    static_assert( sizeof( chunk ) == 8, "chunk must be 64-bit" );
    static_assert( sizeof( mul_chunk ) == 16, "mul_chunk must be 128-bit" );
    static_assert( std::is_same_v<digit, uint8_t>,
                   "digit type consistency check" );
}
