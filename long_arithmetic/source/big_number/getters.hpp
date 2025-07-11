#pragma once

#include <cstdint>

#include "big_number.hpp"

namespace big_number {
    const Error& get_error( const BigNumber& number );

    int32_t get_shift( const BigNumber& number );

    size_t get_size( const BigNumber& number );

    BigNumberType get_type( const BigNumber& number );

    const chunks& get_mantissa( const BigNumber& number );

    chunk get_chunk( const BigNumber& number, size_t index );

    chunk get_shifted_chunk( const BigNumber& number, int32_t index );

    int32_t count_power( const BigNumber& number );

    bool is_negative( const BigNumber& number );

    bool is_zero( const BigNumber& number );

    bool is_inf( const BigNumber& number );

    bool is_nan( const BigNumber& number );

    bool is_special( const BigNumber& number );

    bool has_same_sign( const BigNumber& lhs, const BigNumber& rhs );

    const Error& propagate_error( const BigNumber& lhs, const BigNumber& rhs );
}
