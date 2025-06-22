#include <cstdint>

#include "big_number.hpp"
#include "getters.hpp"

namespace big_number {
    bool has_lower_power( const BigNumber& lhs, const BigNumber& rhs ) {
        int32_t lhs_power = count_power( lhs );
        int32_t rhs_power = count_power( rhs );
        return ( lhs_power != rhs_power ) ? lhs_power < rhs_power : false;
    }

    bool is_lower_special( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( is_nan( lhs ) || is_nan( rhs ) ) return false;

        switch ( get_type( lhs ) ) {
        case BigNumberType::INF:
            return is_inf( rhs ) ? is_negative( lhs ) && !is_negative( rhs )
                                 : is_negative( lhs );

        case BigNumberType::ZERO:
            return is_zero( rhs ) ? false : !is_negative( rhs );

        default:
            if ( is_inf( rhs ) ) return !is_negative( rhs );
            if ( is_zero( rhs ) ) return is_negative( lhs );
            return false;
        }
    }

    bool is_lower_than( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( is_special( lhs ) || is_special( rhs ) )
            return is_lower_special( lhs, rhs );

        if ( is_negative( lhs ) != is_negative( rhs ) )
            return is_negative( lhs ) > is_negative( rhs );

        if ( has_lower_power( lhs, rhs ) )
            return is_negative( lhs ) ? false : true;

        const chunks& lhs_mantissa = get_mantissa( lhs );
        const chunks& rhs_mantissa = get_mantissa( rhs );
        size_t lhs_size = lhs_mantissa.size();
        size_t rhs_size = rhs_mantissa.size();
        size_t min_size = std::min( lhs_size, rhs_size );

        if ( lhs_size != rhs_size ) return min_size == lhs_size;

        for ( size_t delta = 1; delta <= min_size; delta++ ) {
            chunk lhs_chunk = lhs_mantissa[lhs_size - delta];
            chunk rhs_chunk = rhs_mantissa[rhs_size - delta];
            if ( lhs_chunk != rhs_chunk ) {
                bool is_chunk_lower = lhs_chunk < rhs_chunk;
                return is_negative( lhs ) ? !is_chunk_lower : is_chunk_lower;
            }
        }
        return false;
    }
}
