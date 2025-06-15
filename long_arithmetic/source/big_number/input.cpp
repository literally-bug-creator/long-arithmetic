#include <cstdint>

#include "big_number.hpp"
#include "constructors.hpp"
#include "raw_number.hpp"

namespace big_number {
    int32_t count_signitificant_digits( RawNumber raw ) noexcept {
        int32_t zeros = get_leading_zeros( raw ) + get_trailing_zeros( raw );
        int32_t size = get_size( raw );

        return ( zeros >= size ) ? 0 : size - zeros;
    }

    int32_t count_chunks( int32_t significant_digits, int32_t offset ) {
        return ( significant_digits + offset + BASE - 1 ) / BASE;
    }

    std::vector<chunk> compute_chunks( const RawNumber& raw_number ) {
        int32_t significant_digits = count_signitificant_digits( raw_number );

        if ( significant_digits == 0 ) return {};

        std::vector<chunk> chunks;
        int32_t offset = get_offset( raw_number );
        chunks.reserve( count_chunks( significant_digits, offset ) );
        int32_t leading_zeros = get_leading_zeros( raw_number );
        size_t pos = significant_digits + leading_zeros + offset;

        while ( pos > leading_zeros ) {
            chunk value = 0;
            chunk factor = 1;

            for ( int i = 0; i < BASE; ++i ) {
                if ( pos > leading_zeros ) {
                    --pos;
                    int32_t digit = get_digit( raw_number, pos );
                    value += static_cast<chunk>( digit ) * factor;
                }

                factor *= 10;
            }

            chunks.push_back( value );
        }

        return chunks;
    }

    int32_t compute_shift( const RawNumber& raw_number ) noexcept {
        return ( get_exp( raw_number ) - get_offset( raw_number ) ) / BASE;
    }

    BigNumber make_big_number( const std::vector<int>& digits,
                               int32_t exp,
                               bool is_negative,
                               const Error& error ) {
        const RawNumber raw =
            make_raw_number( digits, exp, is_negative, error );

        const std::vector<chunk> chunks = compute_chunks( raw );
        const int32_t shift = compute_shift( raw );

        return chunks.empty() ? make_zero( error )
                              : BigNumber{ chunks, error, shift, is_negative };
    }
}
