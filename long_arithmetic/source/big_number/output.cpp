#include <cstdint>
#include <string>

#include "big_number.hpp"
#include "constants.hpp"
#include "getters.hpp"

namespace big_number {
    int32_t compute_exponent( const BigNumber& number ) {
        return get_shift( number ) * BASE;
    }

    std::string get_sign_string( bool is_negative ) {
        return is_negative ? MINUS_STR : EMPTY_STR;
    }

    std::string get_exponent_string( const BigNumber& number ) {
        int32_t exponent = compute_exponent( number );
        if ( exponent == ZERO_INT ) return EMPTY_STR;

        return EXP_STR + get_sign_string( exponent < ZERO_INT ) +
               std::to_string( exponent );
    }

    std::string format_chunk( chunk value, bool is_first ) {
        std::string str = std::to_string( value );
        if ( is_first ) return str;

        str.insert( str.begin(), BASE - str.length(), ZERO_CHAR );
        return str;
    }

    std::string get_mantissa_string( const BigNumber& number ) {
        size_t size = get_size( number );
        if ( size == ZERO_INT ) return EMPTY_STR;

        const chunks& mantissa = get_mantissa( number );
        size_t first_chunk_idx = size - ONE_INT;
        std::string str;

        str.reserve( size * BASE );

        for ( size_t index = size - ONE_INT; index > ZERO_INT; --index ) {
            str += format_chunk( mantissa[index], first_chunk_idx == index );
        }
        str += format_chunk( mantissa[0], 0 == first_chunk_idx );

        return str;
    }

    std::string get_special_string( const BigNumber& number ) {
        switch ( get_type( number ) ) {
        case BigNumberType::NOT_A_NUMBER:
            return NAN_STR;
        case BigNumberType::INF:
            return get_sign_string( is_negative( number ) ) + INF_STR;
        case BigNumberType::ZERO:
            return ZERO_STR;
        default:
            return EMPTY_STR;
        }
    }

    std::string to_string( const BigNumber& number ) {
        if ( is_special( number ) ) return get_special_string( number );

        std::string sign = get_sign_string( is_negative( number ) );
        std::string mantissa = get_mantissa_string( number );
        std::string exponent = get_exponent_string( number );

        return sign + mantissa + exponent;
    }
}
