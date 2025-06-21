#include <cstdint>
#include <iostream>
#include <string>

#include "big_number.hpp"
#include "constants.hpp"
#include "getters.hpp"

namespace big_number {
    int32_t compute_exp( const BigNumber& number ) {
        return get_shift( number ) * BASE;
    }

    std::string to_string_sign( bool is_negative ) {
        return is_negative ? MINUS_STR : EMPTY_STR;
    }

    std::string to_string_exp( const BigNumber& number ) {
        int32_t exp = compute_exp( number );
        if ( exp == ZERO_INT ) return EMPTY_STR;

        return EXP_STR + to_string_sign( exp < ZERO_INT ) +
               std::to_string( exp );
    }

    std::string to_string_chunk( chunk value, bool is_first ) {
        std::string str = std::to_string( value );
        if ( is_first ) return str;

        while ( str.length() < BASE ) {
            str = ZERO_CHAR + str;
        }
        return str;
    }

    std::string to_string_mantissa( const BigNumber& number ) {
        size_t size = get_size( number );
        if ( size == ZERO_INT ) return EMPTY_STR;

        const chunks& mantissa = get_mantissa( number );
        size_t first_chunk_idx = size - ONE_INT;
        std::string str;

        for ( size_t index = size - ONE_INT; index > ZERO_INT; --index ) {
            chunk value = mantissa[index];
            str += to_string_chunk( value, first_chunk_idx == index );
        }
        str += to_string_chunk( mantissa[0], 0 == first_chunk_idx );

        return str;
    }

    std::string to_string_special( const BigNumber& number ) {
        switch ( get_type( number ) ) {
        case BigNumberType::NOT_A_NUMBER:
            return NAN_STR;

        case BigNumberType::INF:
            return to_string_sign( is_negative( number ) ) + INF_STR;

        case BigNumberType::ZERO:
            return ZERO_STR;

        default:
            return EMPTY_STR;
        }
    }

    std::string to_string( const BigNumber& number ) {
        if ( is_special( number ) ) return to_string_special( number );

        std::string sign = to_string_sign( is_negative( number ) );
        std::string mantissa = to_string_mantissa( number );
        std::string exp = to_string_exp( number );

        return sign + mantissa + exp;
    }
}
