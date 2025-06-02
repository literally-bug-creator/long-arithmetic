#include <cstddef>
#include <cstdint>

#include "big_number.hpp"
#include "error.hpp"

BigNumber make_big_number( const std::string& str );

BigNumber from_scratch( std::vector<chunk>& chunks,
                        int32_t exponent,
                        bool is_negative,
                        Error& error ) {
    return BigNumber{ .chunks = chunks,
                      .exponent = exponent,
                      .is_negative = is_negative,
                      .error = error };
}

BigNumber from_iterator( std::vector<chunk>::const_iterator begin,
                         std::vector<chunk>::const_iterator end ) {
    size_t size = end - begin;
    int32_t exponent = 0;
    std::vector<chunk> chunks( 0 );

    while ( size > 0 && *begin == 0 ) {
        ++begin;
        --size;
        ++exponent;
    }

    while ( size > 0 && *( end - 1 ) == 0 ) {
        --end;
        size--;
    }
    if ( size > 0 ) { chunks = std::vector<chunk>( begin, end ); }

    Error error = make_error( OK, "" );

    return from_scratch( chunks, exponent, false, error );
}

BigNumber make_zero() { return BigNumber( {}, 0, 0, make_error( OK, "" ) ); }

std::string to_string( const BigNumber& number );
