#pragma once

#include "big_number.hpp"

namespace big_number {
    const BigNumber ZERO = make_big_number( {}, 0, false );
    const BigNumber ONE = make_big_number( { 1 }, 0, false );
    const BigNumber TWO = make_big_number( { 2 }, 0, false );

    BigNumber make_zero( const Error& error = get_default_error() );

    BigNumber internal_make_big_number( std::vector<chunk> chunks,
                               int32_t exponent,
                               bool is_negative,
                               const Error& error );
}
