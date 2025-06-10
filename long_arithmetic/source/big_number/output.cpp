#include "big_number.hpp"

namespace big_number {
    const std::string MINUS_STR = "-";

    std::string to_string( const BigNumber& number ) {
        if ( number.chunks.empty() ) return std::to_string( ZERO_INT );

        std::string result;
        if ( number.is_negative ) result += MINUS_STR;

        for ( int i = static_cast<int>( number.chunks.size() ) - ONE_INT;
              i >= ZERO_INT;
              --i ) {
            std::string chunk_str = std::to_string( number.chunks[i] );

            if ( i == static_cast<int>( number.chunks.size() ) - ONE_INT ) {
                result += chunk_str;
            } else {
                result += std::string( CHUNK_DIGITS - chunk_str.size(), '0' ) +
                          chunk_str;
            }
        }

        if ( number.exponent != ZERO_INT ) {
            result += 'e' + std::to_string( number.exponent * CHUNK_DIGITS );
        }

        return result;
    }
}
