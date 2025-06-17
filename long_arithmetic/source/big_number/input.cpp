#include "big_number.hpp"
#include "constructors.hpp"

namespace big_number {
    size_t count_chunks( const digits& value, digit offset ) noexcept {
        if ( ( value.empty() ) || ( value[0] == 0 ) ) return 0;
        size_t amount = ( ( value.size() + offset ) + BASE - 1 ) / BASE;
        return ( amount > MAX_INPUT_CHUNKS ) ? 0 : amount;
    }

    digit get_digit( const digits& digits, size_t index ) noexcept {
        return ( index >= digits.size() ) ? 0 : digits[index];
    }

    int32_t compute_shift( int32_t exp, digit offset ) noexcept {
        return ( exp - offset ) / BASE;
    }

    chunks convert_to_chunks( const digits& digits, digit offset ) noexcept {
        chunks value( count_chunks( digits, offset ) );
        int32_t index = static_cast<int32_t>( digits.size() ) + offset - 1;

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
        return ( remainder < 0 ) ? remainder + BASE : remainder;
    }

    BigNumber make_big_number( const digits& digits,
                               int32_t exp,
                               bool is_negative,
                               const Error& error ) noexcept {
        digit offset = compute_offset( exp );
        chunks mantissa = convert_to_chunks( digits, offset );
        int32_t shift = compute_shift( exp, offset );
        BigNumberType type = BigNumberType::DEFAULT;

        return make_big_number( mantissa, shift, type, error, is_negative );
    }
}
