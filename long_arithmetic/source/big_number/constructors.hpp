#pragma once

#include "big_number.hpp"

namespace big_number {
    const std::string EMPTY_STR = "";
    const Error DEFAULT_ERROR = make_error( OK, EMPTY_STR );
    const BigNumber ZERO = make_big_number( "0" );
    const BigNumber ONE = make_big_number( "1" );
    const BigNumber TWO = make_big_number( "2" );

    BigNumber from_scratch( const std::vector<chunk>& chunks,
                            int32_t exponent,
                            bool is_negative,
                            const Error& error );

    BigNumber make_zero( Error error = DEFAULT_ERROR );

    std::string to_string( const BigNumber& number );
}
