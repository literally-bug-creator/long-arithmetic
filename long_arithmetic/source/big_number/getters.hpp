#pragma once

#include "big_number.hpp"

namespace big_number {
    const Error& get_error( const BigNumber& number );

    int32_t get_shift( const BigNumber& number );

    size_t get_size( const BigNumber& number );

    const std::vector<chunk>& get_chunks( const BigNumber& number );

    chunk get_chunk( const BigNumber& number, int32_t shift );

    bool is_negative( const BigNumber& number );

    bool is_zero( const BigNumber& number );
}
