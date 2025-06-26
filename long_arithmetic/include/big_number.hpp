#pragma once

#include <cmath>
#include <inttypes.h>
#include <vector>

#include "error.hpp"

namespace big_number {
    typedef __uint128_t mul_chunk;
    typedef uint64_t chunk;

    const int CHUNK_DIGITS = 18;
    const chunk CHUNK_BASE = static_cast<chunk>( std::pow( 10, CHUNK_DIGITS ) );
    const chunk MAX_CHUNK = CHUNK_BASE - 1;
    const int32_t DIV_PRECISION = 10;

    struct BigNumber {
        std::vector<chunk> chunks;
        int32_t exponent;
        bool is_negative;
        Error error;

        // Move constructor
        BigNumber(BigNumber&& other) noexcept;
        // Move assignment
        BigNumber& operator=(BigNumber&& other) noexcept;
        // Copy constructor/assignment по умолчанию
        BigNumber(const BigNumber&) = default;
        BigNumber& operator=(const BigNumber&) = default;
        // Default constructor
        BigNumber() = default;
        // Быстрый конструктор из int
        BigNumber(int value);
        ~BigNumber() = default;
    };

    const Error& get_error( const BigNumber& number );

    BigNumber make_big_number( const std::string& str );

    bool is_equal( const BigNumber& left, const BigNumber& right );

    bool is_lower_than( const BigNumber& left, const BigNumber& right );

    bool is_greater_than( const BigNumber& left, const BigNumber& right );

    BigNumber abs( const BigNumber& operand );

    BigNumber neg( const BigNumber& operand );

    BigNumber add( const BigNumber& augend, const BigNumber& addend );

    BigNumber sub( const BigNumber& minuend, const BigNumber& subtrahend );

    BigNumber mul( const BigNumber& multiplicand, const BigNumber& multiplier );

    BigNumber div( const BigNumber& dividend,
                   const BigNumber& divisor,
                   int32_t precision = DIV_PRECISION );

    BigNumber sqrt( const BigNumber& radicand );

    std::string to_string( const BigNumber& number );

    // Арифметические операторы
    BigNumber operator+(const BigNumber& a, const BigNumber& b);
    BigNumber operator-(const BigNumber& a, const BigNumber& b);
    BigNumber operator*(const BigNumber& a, const BigNumber& b);
    BigNumber operator/(const BigNumber& a, const BigNumber& b);
    // Операторы сравнения
    bool operator==(const BigNumber& a, const BigNumber& b);
    bool operator!=(const BigNumber& a, const BigNumber& b);
    bool operator<(const BigNumber& a, const BigNumber& b);
    bool operator>(const BigNumber& a, const BigNumber& b);
    bool operator<=(const BigNumber& a, const BigNumber& b);
    bool operator>=(const BigNumber& a, const BigNumber& b);
}
