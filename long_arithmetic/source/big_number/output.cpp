#include "big_number.hpp"
#include "getters.hpp"

namespace big_number {
    const std::string EMPTY_STR = "";
    const std::string ZERO_STR = "0";
    const std::string MINUS_STR = "-";
    const char DOT_CHAR = '.';
    const char ZERO_CHAR = '0';

    std::string get_chunk_string( const BigNumber& number,
                                  int32_t position,
                                  bool is_first ) {
        int32_t chunk_index = position - number.exponent;

        if ( chunk_index < ZERO_INT ||
             chunk_index >= static_cast<int32_t>( number.chunks.size() ) ) {
            return is_first ? ZERO_STR : std::string( CHUNK_DIGITS, ZERO_CHAR );
        }

        std::string chunk_str = std::to_string( number.chunks[chunk_index] );

        if ( !is_first && chunk_str.size() < CHUNK_DIGITS ) {
            chunk_str =
                std::string( CHUNK_DIGITS - chunk_str.size(), ZERO_CHAR ) +
                chunk_str;
        }

        return chunk_str;
    }

    std::string trim_decimal_zeros( std::string str ) {
        if ( str.find( DOT_CHAR ) != std::string::npos ) {
            while ( str.back() == ZERO_CHAR )
                str.pop_back();
            if ( str.back() == DOT_CHAR ) str.pop_back();
        }
        return str;
    }

    std::string to_string( const BigNumber& number ) {
        if ( is_zero( number ) ) return ZERO_STR;

        std::string result = number.is_negative ? MINUS_STR : EMPTY_STR;

        int32_t max_pos = std::max(
            number.exponent + static_cast<int32_t>( number.chunks.size() ) -
                ONE_INT,
            ZERO_INT );
        int32_t min_pos = std::min( number.exponent, ZERO_INT );

        for ( int32_t pos = max_pos; pos >= min_pos; --pos ) {
            if ( pos == -ONE_INT ) result += DOT_CHAR;
            result += get_chunk_string( number, pos, pos == max_pos );
        }

        return min_pos < ZERO_INT ? trim_decimal_zeros( result ) : result;
    }
}
