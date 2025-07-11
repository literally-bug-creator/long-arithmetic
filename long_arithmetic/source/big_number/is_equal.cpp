#include "big_number.hpp"
#include "getters.hpp"

namespace big_number {
    bool has_equal_mantissa( const BigNumber& lhs, const BigNumber& rhs ) {
        const chunks& lhs_mantissa = get_mantissa( lhs );
        const chunks& rhs_mantissa = get_mantissa( rhs );

        return ( lhs_mantissa.size() != rhs_mantissa.size() )
                   ? false
                   : lhs_mantissa == rhs_mantissa;
    }

    bool has_equal_shift( const BigNumber& lhs, const BigNumber& rhs ) {
        return get_shift( lhs ) == get_shift( rhs );
    }

    bool has_equal_sign( const BigNumber& lhs, const BigNumber& rhs ) {
        return is_negative( lhs ) == is_negative( rhs );
    }

    bool has_equal_type( const BigNumber& lhs, const BigNumber& rhs ) {
        return get_type( lhs ) == get_type( rhs );
    }

    bool is_any_nan( const BigNumber& left, const BigNumber& right ) {
        return is_nan( left ) || is_nan( right );
    }

    bool is_equal( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( is_any_nan( lhs, rhs ) ) return false;
        return has_equal_type( lhs, rhs ) && has_equal_sign( lhs, rhs ) &&
               has_equal_shift( lhs, rhs ) && has_equal_mantissa( lhs, rhs );
    }
}
