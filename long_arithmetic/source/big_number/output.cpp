#include "big_number.hpp"

namespace big_number {
    bool is_zero( const BigNumber& number ) {
        return number.chunks.empty() ||
               ( number.chunks.size() == 1 && number.chunks[0] == 0 );
    }

    std::string get_chunk_string( const BigNumber& number,
                                  int32_t position,
                                  bool is_first ) {
        int32_t chunk_index = position - number.exponent;

        if ( chunk_index < 0 ||
             chunk_index >= static_cast<int32_t>( number.chunks.size() ) ) {
            return is_first ? "0" : std::string( CHUNK_DIGITS, '0' );
        }

        std::string chunk_str = std::to_string( number.chunks[chunk_index] );

        if ( !is_first && chunk_str.size() < CHUNK_DIGITS ) {
            chunk_str =
                std::string( CHUNK_DIGITS - chunk_str.size(), '0' ) + chunk_str;
        }

        return chunk_str;
    }

    std::string trim_decimal_zeros( std::string str ) {
        if ( str.find( '.' ) != std::string::npos ) {
            while ( str.back() == '0' )
                str.pop_back();
            if ( str.back() == '.' ) str.pop_back();
        }
        return str;
    }

    std::string to_string( const BigNumber& number ) {
        if ( is_zero( number ) ) return "0";

        std::string result = number.is_negative ? "-" : "";

        int32_t max_pos = std::max(
            number.exponent + static_cast<int32_t>( number.chunks.size() ) - 1,
            0 );
        int32_t min_pos = std::min( number.exponent, 0 );

        for ( int32_t pos = max_pos; pos >= min_pos; --pos ) {
            if ( pos == -1 ) result += '.';
            result += get_chunk_string( number, pos, pos == max_pos );
        }

        return min_pos < 0 ? trim_decimal_zeros( result ) : result;
    }
}
