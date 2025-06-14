#pragma once

#include "big_number.hpp"

namespace big_number {
    BigNumber make_zero( const Error& error );

    BigNumber internal_make_big_number( std::vector<chunk> chunks,
                                        int32_t exponent,
                                        bool is_negative,
                                        const Error& error );
}
