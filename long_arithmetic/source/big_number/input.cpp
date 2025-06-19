#include <cstdlib>

#include "big_number.hpp"
#include "constants.hpp"
#include "constructors.hpp"

namespace big_number {
    size_t count_chunks( const digits& value, digit offset ) {
        if ( ( value.empty() ) || ( value[0] == 0 ) ) return 0;
        return ( ( value.size() + offset ) + BASE - ONE_INT ) / BASE;
    }

    bool is_overflowed( int32_t exp ) { return abs( exp ) > MAX_EXP; }

    digit get_digit( const digits& digits, size_t index ) {
        return ( index >= digits.size() ) ? ZERO_INT : digits[index];
    }

    int32_t compute_shift( int32_t exp, digit offset ) {
        // return ( is_overflowed( exp ) ) ? ( MAX_SHIFT + ONE_INT )
        //                                 : ( exp - offset ) / BASE;
        return ( exp - offset ) / BASE;
    }

    chunks convert_to_chunks( const digits& digits, digit offset ) {
        chunks value( count_chunks( digits, offset ) );
        int32_t index =
            static_cast<int32_t>( digits.size() ) + offset - ONE_INT;

        for ( chunk& value : value ) {
            int32_t to = std::max( -1, index - BASE );
            chunk factor = 1;

            for ( int32_t i = index; i > to; i-- ) {
                int32_t digit = get_digit( digits, i );
                value += static_cast<chunk>( digit ) * factor;
                factor *= 10;
            }
            index = to;
        }
        return value;
    }

    digit compute_offset( int32_t exp ) noexcept {
        int8_t remainder = static_cast<int8_t>( exp % BASE );
        return ( remainder < ZERO_INT ) ? ( remainder + BASE ) : remainder;
    }

    digits normalize( const digits& value ) {
        digits normalized = value;
        if ( normalized.size() > MAX_DIGITS ) {
            normalized.resize( MAX_DIGITS );
        }
        return normalized;
    }

    int32_t
    compensate_exp( int32_t exp, const digits& raw, const digits& normalized ) {
        size_t delta = raw.size() - normalized.size();
        return ( delta > MAX_EXP ) ? ( MAX_EXP + ONE_INT ) : ( exp + delta );
    }

    BigNumber make_big_number( const digits& raw_digits,
                               int32_t raw_exp,
                               bool is_negative,
                               const Error& error ) {
        digits digits = normalize( raw_digits );
        int32_t exp = compensate_exp( raw_exp, raw_digits, digits );
        digit offset = compute_offset( exp );

        return make_big_number( convert_to_chunks( digits, offset ),
                                compute_shift( exp, offset ),
                                BigNumberType::DEFAULT,
                                error,
                                is_negative );
    }
}
