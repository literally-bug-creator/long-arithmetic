#include <cstdint>

#include "big_number.hpp"
#include "getters.hpp"

namespace big_number {
    bool has_lower_than_mantissa( const BigNumber& lhs, const BigNumber& rhs ) {
        const chunks& lhs_mantissa = get_mantissa( lhs );
        const chunks& rhs_mantissa = get_mantissa( rhs );
        size_t lhs_size = lhs_mantissa.size();
        size_t rhs_size = rhs_mantissa.size();
        size_t min_size = std::min( lhs_size, rhs_size );

        if ( lhs_size != rhs_size ) return min_size == lhs_size;

        for ( size_t delta = 0; delta <= min_size; delta++ ) {
            if ( lhs_mantissa[lhs_size - delta] <
                 rhs_mantissa[rhs_size - delta] )
                return true;
        }

        return false;
    }

    bool has_lower_than_size( const BigNumber& lhs, const BigNumber& rhs ) {
        int32_t lhs_size = get_size( lhs ) + get_shift( lhs );
        int32_t rhs_size = get_size( rhs ) + get_shift( rhs );
        return lhs_size < rhs_size;
    }

    bool has_lower_than_sign( const BigNumber& lhs, const BigNumber& rhs ) {
        return is_negative( lhs ) < is_negative( rhs );
    }

    bool is_lower_than_special( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( is_nan( lhs ) || is_nan( rhs ) ) return false;

        switch ( get_type( lhs ) ) {
        case BigNumberType::INF:
            return is_inf( rhs ) ? is_negative( lhs ) && !is_negative( rhs )
                                 : is_negative( lhs );

        case BigNumberType::ZERO:
            return is_zero( rhs ) ? false : is_negative( rhs );

        default:
            if ( is_inf( rhs ) ) return !is_negative( rhs );
            if ( is_zero( rhs ) ) return is_negative( lhs );
            return false;
        }
    }

    bool is_lower_than( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( is_special( lhs ) || is_special( rhs ) )
            return is_lower_than_special( lhs, rhs );

        if ( has_lower_than_sign( lhs, rhs ) ) return true;
        if ( has_lower_than_size( lhs, rhs ) ) return true;
        if ( has_lower_than_mantissa( lhs, rhs ) ) return true;

        return false;
    }
}
