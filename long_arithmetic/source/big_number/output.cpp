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

        return std::string( BASE - chunk_str.size(), ZERO_CHAR ) + chunk_str;
    }

    uint count_trailing_zeros( chunk value ) {
        if ( value == 0 ) { return 0; }

        uint count = 0;

        while ( value % 10000000000ULL == 0 && value != 0 ) {
            value /= 10000000000ULL;
            count += 10;
        }

        while ( value % 100000ULL == 0 && value != 0 ) {
            value /= 100000ULL;
            count += 5;
        }

        while ( value % 10 == 0 && value != 0 ) {
            value /= 10;
            count++;
        }

        return count;
    }

    chunk remove_trailing_zeros_fast( chunk value ) {
        if ( value == 0 ) { return 0; }

        while ( value % 10000000000ULL == 0 && value != 0 ) {
            value /= 10000000000ULL;
        }

        while ( value % 100000ULL == 0 && value != 0 ) {
            value /= 100000ULL;
        }

        while ( value % 10 == 0 ) {
            value /= 10;
        }

        return value;
    }

    std::string to_string_chunks( const BigNumber& number ) {
        int32_t last_chunk_index =
            static_cast<int32_t>( get_size( number ) ) - ONE_INT;
        chunk last_chunk = get_chunk_direct( number, last_chunk_index );
        if ( get_size( number ) <= ONE_INT )
            last_chunk = remove_trailing_zeros_fast( last_chunk );

        std::string str = to_string_chunk( last_chunk, false );

        for ( int32_t index = last_chunk_index - ONE_INT; index > ZERO_INT;
              --index ) {
            chunk value = get_chunk_direct( number, index );
            str += to_string_chunk( value, true );
        }

        if ( get_size( number ) <= ONE_INT ) return str;

        chunk first_chunk = get_chunk_direct( number, ZERO_INT );
        chunk stripped_chunk = remove_trailing_zeros_fast( first_chunk );
        str += std::to_string( stripped_chunk );

        return str;
    }

    int32_t compute_exponent_shift( const BigNumber& number ) {
        chunk first_chunk = get_chunk_direct( number, ZERO_INT );
        return count_trailing_zeros( first_chunk );
    }

    std::string to_string_exp( const BigNumber& number ) {
        int32_t exp = get_exponent( number ) * BASE;
        exp += compute_exponent_shift( number );

        if ( exp == ZERO_INT ) return EMPTY_STR;
        return EXPONENT_CHAR + std::to_string( exp );
    }

    std::string to_string( const BigNumber& number ) {
        if ( is_zero( number ) ) return std::to_string( ZERO_INT );

        return to_string_sign( number ) + to_string_chunks( number ) +
               to_string_exp( number );
    }
}
