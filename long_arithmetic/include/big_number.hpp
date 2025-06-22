#pragma once

#include <string>

#include "constants.hpp"
#include "error.hpp"

namespace big_number {
    enum class BigNumberType : uint8_t {
        DEFAULT,
        ZERO,
        INF,
        NOT_A_NUMBER,
    };

    struct BigNumber {
        chunks mantissa;
        int32_t shift;
        BigNumberType type;
        Error error;
        bool is_negative;
    };

    BigNumber make_big_number( digits digits,
                               int32_t exponent,
                               bool is_negative,
                               const Error& error );

    BigNumber make_zero( const Error& error );

    BigNumber make_nan( const Error& error );

    BigNumber make_inf( const Error& error, bool is_negative );

    const Error& get_error( const BigNumber& number );

    BigNumber abs( const BigNumber& number );

    BigNumber neg( const BigNumber& number );

    bool is_equal( const BigNumber& left, const BigNumber& right );

    std::string to_string( const BigNumber& number );
}
