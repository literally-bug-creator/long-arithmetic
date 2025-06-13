#include <string>

#include "big_number.hpp"
#include "getters.hpp"

namespace big_number {
    const std::string EMPTY_STR = "";
    const std::string MINUS_STR = "-";
    const char ZERO_CHAR = '0';
    const char EXPONENT_CHAR = 'e';

    std::string to_string_sign( const BigNumber& number ) {
        return is_negative( number ) ? MINUS_STR : EMPTY_STR;
    }

    std::string to_string_chunk( chunk value, bool should_filled ) {
        std::string chunk_str = std::to_string( value );

        if ( !should_filled ) { return chunk_str; }

        return std::string( BASE - chunk_str.size(), ZERO_CHAR ) +
               chunk_str;
    }

    std::string to_string_chunks( const BigNumber& number ) {
        std::string str;
        int32_t last_chunk_index =
            static_cast<int32_t>( get_size( number ) ) - ONE_INT;

        for ( int32_t index = last_chunk_index; index >= ZERO_INT; --index ) {
            chunk value = get_chunk_direct( number, index );
            str += to_string_chunk( value, index != last_chunk_index );
        }

        return str;
    }

    std::string to_string_exp( const BigNumber& number ) {
        int32_t exp = get_exponent( number );

        if ( exp == ZERO_INT ) return EMPTY_STR;

        return EXPONENT_CHAR + std::to_string( exp * BASE );
    }

    std::string to_string( const BigNumber& number ) {
        if ( is_zero( number ) ) return std::to_string( ZERO_INT );

        return to_string_sign( number ) + to_string_chunks( number ) +
               to_string_exp( number );
    }
}
