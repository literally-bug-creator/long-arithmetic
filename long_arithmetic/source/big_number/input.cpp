#include "big_number.hpp"
#include "constants.hpp"
#include "constructors.hpp"

namespace big_number {
    digit get_digit( const digits& digits, size_t index ) {
        return ( index >= digits.size() ) ? ZERO_INT : digits[index];
    }

    size_t count_chunks( const digits& value, digit offset ) {
        if ( value.empty() || value[0] == 0 ) return 0;
        return ( value.size() + offset + BASE - ONE_INT ) / BASE;
    }

    int32_t compute_shift( int32_t exp, digit offset ) {
        return ( exp - offset ) / BASE;
    }

    digit compute_offset( int32_t exp ) {
        int8_t remainder = static_cast<int8_t>( exp % BASE );
        return ( remainder < ZERO_INT ) ? ( remainder + BASE ) : remainder;
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

    bool has_overflow( const digits& value ) {
        return value.size() > MAX_DIGITS;
    }

    bool has_overflow( int32_t exponent ) {
        return std::abs( exponent ) > MAX_EXP;
    }

    BigNumber make_from_overflowed_digits( int32_t raw_exp,
                                           bool is_negative,
                                           const Error& error ) {
        return make_inf( error, is_negative );
    }

    BigNumber make_from_overflowed_exp( int32_t raw_exp,
                                        bool is_negative,
                                        const Error& error ) {
        return ( raw_exp < ZERO_INT ) ? make_zero( error )
                                      : make_inf( error, is_negative );
    }

    BigNumber make_big_number( digits raw_digits,
                               int32_t raw_exp,
                               bool is_negative,
                               const Error& error ) {
        if ( has_overflow( raw_digits ) )
            return make_from_overflowed_digits( raw_exp, is_negative, error );

        if ( has_overflow( raw_exp ) )
            return make_from_overflowed_exp( raw_exp, is_negative, error );

        digit offset = compute_offset( raw_exp );

        return make_big_number( convert_to_chunks( raw_digits, offset ),
                                compute_shift( raw_exp, offset ),
                                BigNumberType::DEFAULT,
                                error,
                                is_negative );
    }
}
