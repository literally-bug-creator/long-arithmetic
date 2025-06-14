#include "big_number.hpp"
#include "constructors.hpp"

namespace big_number {
    size_t count_leading_zeros( const std::vector<int>& digits ) noexcept {
        size_t count = 0;
        for ( auto it = digits.begin(); it != digits.end(); ++it ) {
            if ( *it != 0 ) break;
            ++count;
        }
        return count;
    }

    void fill_chunks( const std::vector<int>& digits,
                      std::vector<chunk>& chunks,
                      int32_t offset,
                      size_t leading_zeros,
                      size_t trailing_zeros ) {
        size_t pos = digits.size() - trailing_zeros;

        while ( pos > leading_zeros && ( pos > 0 || offset > 0 ) ) {
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

    int32_t compute_offset( int32_t exp ) noexcept {
        int32_t remainder = exp % BASE;
        return ( remainder < 0 ) ? remainder + BASE : remainder;
    }

    int32_t compute_shift( int32_t exp ) noexcept {
        const int32_t offset = compute_offset( exp );
        return ( exp - offset ) / BASE;
    }

    std::vector<chunk> convert_to_chunks( const std::vector<int>& digits,
                                          int32_t exp,
                                          size_t trailing_zeros ) {
        if ( digits.empty() ) { return {}; }

        const int32_t leading_zeros = count_leading_zeros( digits );
        const int32_t offset = compute_offset( exp );
        const size_t total_len =
            digits.size() + offset - trailing_zeros - leading_zeros;
        std::vector<chunk> chunks;
        chunks.reserve( ( total_len + BASE - 1 ) / BASE );

        fill_chunks( digits, chunks, offset, leading_zeros, trailing_zeros );

        return chunks;
    }

    size_t count_trailing_zeros( const std::vector<int>& digits ) noexcept {
        size_t count = 0;
        for ( auto it = digits.rbegin(); it != digits.rend(); ++it ) {
            if ( *it != 0 ) break;
            ++count;
        }
        return count;
    }

    BigNumber make_big_number( const std::vector<int>& digits,
                               int32_t exponent,
                               bool is_negative,
                               const Error& error ) {
        const size_t trailing_zeros = count_trailing_zeros( digits );
        const std::vector<chunk> chunks =
            convert_to_chunks( digits, exponent, trailing_zeros );
        const int32_t shift = compute_shift( exponent + trailing_zeros );

        return chunks.empty() ? make_zero( error )
                              : BigNumber{ chunks, error, shift, is_negative };
    }
}
