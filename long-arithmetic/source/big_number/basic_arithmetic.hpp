#include "big_number.hpp"

BigNumber abs( const BigNumber& operand );

BigNumber neg( const BigNumber& operand );

bool is_equal( const BigNumber& left, const BigNumber& right );

// int32_t choose_max_exp( const BigNumber& a, const BigNumber& b );

// int32_t choose_min_exp( const BigNumber& a, const BigNumber& b );

bool is_lower_than( const BigNumber& left, const BigNumber& right );

BigNumber add( const BigNumber& augend, const BigNumber& addend );

BigNumber sub( const BigNumber& minuend, const BigNumber& subtrahend );

BigNumber shift( const BigNumber& operand, int32_t exp );
