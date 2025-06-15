#pragma once

#include <cstdint>
#include <vector>

#include "error.hpp"

namespace big_number {
    struct RawNumber {
        const std::vector<int>& digits;
        const Error& error;
        int32_t exp;
        int32_t leading_zeros;
        int32_t trailing_zeros;
        int32_t offset;
        bool is_negative;
    };

    RawNumber make_raw_number( const std::vector<int>& digits,
                               int32_t exp,
                               bool is_negative,
                               const Error& error );

    size_t get_size( const RawNumber& number );

    int32_t get_exp( const RawNumber& number );

    int32_t get_leading_zeros( const RawNumber& number );

    int32_t get_trailing_zeros( const RawNumber& number );

    int32_t get_offset( const RawNumber& number );

    bool is_negative( const RawNumber& number );

    int32_t get_digit( const RawNumber& number, size_t index );

    const Error& get_error( const RawNumber& number );
}
