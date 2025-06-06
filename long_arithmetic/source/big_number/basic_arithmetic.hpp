#include "big_number.hpp"

namespace big_number {
    BigNumber abs( const BigNumber& operand );

    BigNumber neg( const BigNumber& operand );

    bool is_equal( const BigNumber& left, const BigNumber& right );

    bool is_lower_than( const BigNumber& left, const BigNumber& right );

    bool is_greater_than( const BigNumber& left, const BigNumber& right );

    BigNumber add( const BigNumber& augend, const BigNumber& addend );

    BigNumber sub( const BigNumber& minuend, const BigNumber& subtrahend );

    BigNumber shift( const BigNumber& operand, int32_t exp );
}
