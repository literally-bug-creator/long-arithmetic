#include <array>
#include <cstdlib>

#include "big_number.hpp"
#include "constructors.hpp"

namespace big_number {
    constexpr std::array<chunk, BASE> POWERS_OF_10 = []() {
        std::array<chunk, BASE> res{};
        res[0] = 1;
        for ( int i = 1; i < BASE; ++i ) {
            res[i] = res[i - 1] * 10;
        }
        return res;
    }();

    constexpr int32_t compute_offset( int32_t exp ) noexcept {
        return ( ( exp % BASE ) + BASE ) % BASE;
    }

    constexpr int32_t compute_shift( int32_t exp ) noexcept {
        const int32_t offset = compute_offset( exp );
        const int32_t unsigned_shift = std::abs( exp - offset ) / BASE;
        return unsigned_shift * ( exp < 0 ? -1 : 1 );
    }

    std::vector<chunk> convert_to_chunks( const std::vector<int>& digits,
                                          int32_t exp ) {
        if ( digits.empty() ) { return {}; }

        const int32_t offset = compute_offset( exp );

        const size_t total_length = digits.size() + offset;
        std::vector<chunk> chunks;
        chunks.reserve( ( total_length + BASE - 1 ) / BASE );

        size_t digit_pos = digits.size();
        int32_t curr_offset = offset;

        while ( digit_pos > 0 || curr_offset > 0 ) {
            chunk current_chunk = 0;

            for ( int i = 0; i < BASE; ++i ) {
                if ( curr_offset > 0 ) {
                    --curr_offset;
                } else if ( digit_pos > 0 ) {
                    --digit_pos;
                    current_chunk += static_cast<chunk>( digits[digit_pos] ) *
                                     POWERS_OF_10[i];
                }
            }

            chunks.push_back( current_chunk );
        }

        return chunks;
    }

    BigNumber make_big_number( const std::vector<int>& digits,
                               int32_t exponent,
                               bool is_negative,
                               const Error& error ) {
        const auto chunks = convert_to_chunks( digits, exponent );
        const int32_t shift = compute_shift( exponent );

        if ( chunks.empty() ) { return make_zero( error ); }

        return BigNumber{ chunks, error, shift, is_negative };
    }
}
