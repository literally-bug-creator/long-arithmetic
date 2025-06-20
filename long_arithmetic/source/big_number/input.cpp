#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "big_number.hpp"
#include "constants.hpp"
#include "constructors.hpp"

namespace big_number {
    size_t count_chunks( const digits& value, digit offset ) {
        if ( value.empty() || value[0] == 0 ) return 0;
        return ( value.size() + offset + BASE - ONE_INT ) / BASE;
    }

    bool is_overflowed( int32_t exp ) { return std::abs( exp ) > MAX_EXP; }

    digit get_digit( const digits& digits, size_t index ) {
        return ( index >= digits.size() ) ? ZERO_INT : digits[index];
    }

    int32_t compute_shift( int32_t exp, digit offset ) {
        return ( exp - offset ) / BASE;
    }

    chunks convert_to_chunks( const digits& digits, digit offset ) {
        chunks container( count_chunks( digits, offset ) );
        int32_t index =
            static_cast<int32_t>( digits.size() ) + offset - ONE_INT;

        for ( chunk& value : container ) {
            int32_t to = std::max( -1, index - BASE );
            chunk factor = 1;

            for ( int32_t i = index; i > to; i-- ) {
                int32_t digit = get_digit( digits, i );
                value += static_cast<chunk>( digit ) * factor;
                factor *= 10;
            }
            index = to;
        }
        return container;
    }

    digit compute_offset( int32_t exp ) {
        int8_t remainder = static_cast<int8_t>( exp % BASE );
        return ( remainder < ZERO_INT ) ? ( remainder + BASE ) : remainder;
    }

    digits normalize( digits value ) {
        if ( value.size() > MAX_DIGITS ) value.resize( MAX_DIGITS );
        return value;
    }

    int32_t
    compensate_exp( int32_t exp, size_t raw_size, size_t normalized_size ) {
        size_t delta = raw_size - normalized_size;
        return ( delta > MAX_EXP ) ? ( MAX_EXP + ONE_INT ) : ( exp + delta );
    }

    BigNumber make_big_number( digits raw_digits,
                               int32_t raw_exp,
                               bool is_negative,
                               const Error& error ) {
        size_t raw_digits_size = raw_digits.size();
        digits digits = normalize( std::move( raw_digits ) );
        int32_t exp = compensate_exp( raw_exp, raw_digits_size, digits.size() );
        digit offset = compute_offset( exp );

        return make_big_number( convert_to_chunks( digits, offset ),
                                compute_shift( exp, offset ),
                                BigNumberType::DEFAULT,
                                error,
                                is_negative );
    }
}
