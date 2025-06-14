#include "big_number.hpp"
#include "constructors.hpp"

namespace big_number {
    int32_t compute_offset( int32_t exp ) {
        return ( ( exp % BASE ) + BASE ) % BASE;
    }

    int32_t compute_shift( int32_t exp ) {
        int32_t offset = compute_offset( exp );
        int32_t unsigned_shift = ::abs( exp - offset ) / BASE;
        return unsigned_shift * ( exp < 0 ? -1 : 1 );
    }

    std::vector<chunk> convert_to_chunks( const std::vector<int>& digits,
                                          int32_t exp ) {
        if ( digits.size() == 0 ) return {};

        int32_t offset = compute_offset( exp );
        size_t total_length = digits.size() + offset;
        std::vector<chunk> chunks;
        chunks.reserve( ( total_length + BASE - 1 ) / BASE );

        for ( size_t digit_pos = digits.size(); digit_pos > 0 || offset > 0; ) {
            chunk current_chunk = 0;
            chunk power = 1;

            for ( int i = 0; i < BASE; ++i ) {
                if ( offset > 0 ) {
                    --offset;
                } else if ( digit_pos > 0 ) {
                    --digit_pos;
                    current_chunk +=
                        static_cast<chunk>( digits[digit_pos] ) * power;
                }

                power *= 10;
            }

            chunks.push_back( current_chunk );
        }

        return chunks;
    }

    BigNumber make_big_number( const std::vector<int>& digits,
                               int32_t exponent,
                               bool is_negative,
                               const Error& error ) {
        std::vector<chunk> chunks = convert_to_chunks( digits, exponent );
        int32_t shift = compute_shift( exponent );

        if ( chunks.size() == 0 ) return make_zero( error );
        return BigNumber( chunks, error, shift, is_negative );
    }
}
