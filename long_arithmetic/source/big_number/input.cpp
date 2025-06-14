#include <cstdlib>

#include "big_number.hpp"
#include "constructors.hpp"

namespace big_number {
    int32_t compute_offset( int32_t exp ) noexcept {
        return ( ( exp % BASE ) + BASE ) % BASE;
    }

    int32_t compute_shift( int32_t exp ) noexcept {
        const int32_t offset = compute_offset( exp );
        const int32_t shift = std::abs( exp - offset ) / BASE;
        return ( exp < 0 ) ? -shift : shift;
    }

    void fill_chunks( const std::vector<int>& digits,
                      std::vector<chunk>& chunks,
                      int32_t offset ) {
        size_t pos = digits.size();

        while ( pos > 0 || offset > 0 ) {
            chunk value = 0;
            chunk factor = 1;

            for ( int i = 0; i < BASE; ++i ) {
                if ( offset > 0 ) {
                    --offset;
                } else if ( pos > 0 ) {
                    --pos;
                    value += static_cast<chunk>( digits[pos] ) * factor;
                }

                factor *= 10;
            }

            chunks.push_back( value );
        }
    }

    std::vector<chunk> convert_to_chunks( const std::vector<int>& digits,
                                          int32_t exp ) {
        if ( digits.empty() ) { return {}; }

        const int32_t offset = compute_offset( exp );
        const size_t total_len = digits.size() + offset;
        std::vector<chunk> chunks;
        chunks.reserve( ( total_len + BASE - 1 ) / BASE );

        fill_chunks( digits, chunks, offset );

        return chunks;
    }

    BigNumber make_big_number( const std::vector<int>& digits,
                               int32_t exponent,
                               bool is_negative,
                               const Error& error ) {
        const auto chunks = convert_to_chunks( digits, exponent );
        const int32_t shift = compute_shift( exponent );

        return chunks.empty() ? make_zero( error )
                              : BigNumber{ chunks, error, shift, is_negative };
    }
}
