#include "big_number.hpp"

#include <cstddef>
#include <cstdint>

const Error& get_error( const BigNumber& number );

const std::vector<chunk> get_chunks( const BigNumber& number );

int32_t get_exponent( const BigNumber& number );

bool is_negative( const BigNumber& number );

bool is_zero( const BigNumber& number );

BigNumber abs( const BigNumber& number );

size_t get_size( const BigNumber& number );

BigNumber make_big_number( const std::string& str );

bool is_equal( const BigNumber& left, const BigNumber& right );

bool is_lower_than( const BigNumber& left, const BigNumber& right );

BigNumber neg( const BigNumber& operand );

BigNumber add( const BigNumber& augend, const BigNumber& addend );

BigNumber sub( const BigNumber& minuend, const BigNumber& subtrahend );

BigNumber mul( const BigNumber& multiplicand, const BigNumber& multiplier );

BigNumber div( const BigNumber& dividend, const BigNumber& divisor );

BigNumber sqrt( const BigNumber& radicand );

std::string to_string( const BigNumber& number );
