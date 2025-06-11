#include <cmath>
#include <gtest/gtest.h>
#include <random>

#include "big_number.hpp"

using namespace big_number;

class BigNumberMultiplicationTest : public ::testing::Test {
protected:
    void SetUp() override { rng.seed( 42 ); }

    BigNumber create_from_double( double value ) {
        if ( value == 0.0 ) { return make_big_number( { 0 }, 0, false ); }

        bool negative = value < 0;
        value = std::abs( value );

        uint64_t int_value = static_cast<uint64_t>( value );

        std::vector<chunk> chunks;
        if ( int_value == 0 ) {
            chunks.push_back( 0 );
        } else {
            while ( int_value > 0 ) {
                chunks.push_back( int_value % CHUNK_BASE );
                int_value /= CHUNK_BASE;
            }
        }

        return make_big_number( chunks, 0, negative );
    }

    double extract_to_double( const BigNumber& number ) {
        if ( number.chunks.empty() ||
             ( number.chunks.size() == 1 && number.chunks[0] == 0 ) ) {
            return 0.0;
        }

        double result = 0.0;
        double base_power = 1.0;

        for ( size_t i = 0; i < number.chunks.size(); ++i ) {
            result += number.chunks[i] * base_power;
            base_power *= CHUNK_BASE;
        }

        result *= std::pow( 10.0, number.exponent );

        return number.is_negative ? -result : result;
    }

    bool
    are_approximately_equal( double a, double b, double tolerance = 1e-10 ) {
        if ( std::abs( a ) < tolerance && std::abs( b ) < tolerance ) {
            return true;
        }
        return std::abs( a - b ) <=
               tolerance * std::max( std::abs( a ), std::abs( b ) );
    }

    std::mt19937 rng;
};

TEST_F( BigNumberMultiplicationTest, MultiplyPositiveNumbers ) {
    double a_double = 123.0;
    double b_double = 456.0;
    BigNumber a = create_from_double( a_double );
    BigNumber b = create_from_double( b_double );
    double expected_double = a_double * b_double;

    BigNumber result = mul( a, b );

    ASSERT_TRUE( is_ok( get_error( result ) ) );
    double actual_double = extract_to_double( result );
    EXPECT_TRUE( are_approximately_equal( actual_double, expected_double ) );
}

TEST_F( BigNumberMultiplicationTest, MultiplyNegativeNumbers ) {
    double a_double = -789.0;
    double b_double = -234.0;
    BigNumber a = create_from_double( a_double );
    BigNumber b = create_from_double( b_double );
    double expected_double = a_double * b_double;

    BigNumber result = mul( a, b );

    ASSERT_TRUE( is_ok( get_error( result ) ) );
    double actual_double = extract_to_double( result );
    EXPECT_TRUE( are_approximately_equal( actual_double, expected_double ) );
}

TEST_F( BigNumberMultiplicationTest, MultiplyPositiveByNegative ) {
    double a_double = 567.0;
    double b_double = -890.0;
    BigNumber a = create_from_double( a_double );
    BigNumber b = create_from_double( b_double );
    double expected_double = a_double * b_double;

    BigNumber result = mul( a, b );

    ASSERT_TRUE( is_ok( get_error( result ) ) );
    double actual_double = extract_to_double( result );
    EXPECT_TRUE( are_approximately_equal( actual_double, expected_double ) );
}

TEST_F( BigNumberMultiplicationTest, MultiplyByZero ) {
    double a_double = 12345.0;
    double b_double = 0.0;
    BigNumber a = create_from_double( a_double );
    BigNumber b = create_from_double( b_double );
    double expected_double = a_double * b_double;

    BigNumber result = mul( a, b );

    ASSERT_TRUE( is_ok( get_error( result ) ) );
    double actual_double = extract_to_double( result );
    EXPECT_TRUE( are_approximately_equal( actual_double, expected_double ) );
}

TEST_F( BigNumberMultiplicationTest, MultiplyByOne ) {
    double a_double = 9876.0;
    double b_double = 1.0;
    BigNumber a = create_from_double( a_double );
    BigNumber b = create_from_double( b_double );
    double expected_double = a_double * b_double;

    BigNumber result = mul( a, b );

    ASSERT_TRUE( is_ok( get_error( result ) ) );
    double actual_double = extract_to_double( result );
    EXPECT_TRUE( are_approximately_equal( actual_double, expected_double ) );
}

TEST_F( BigNumberMultiplicationTest, MultiplicationIsCommutative ) {
    double a_double = 147.0;
    double b_double = 258.0;
    BigNumber a = create_from_double( a_double );
    BigNumber b = create_from_double( b_double );

    BigNumber result_ab = mul( a, b );
    BigNumber result_ba = mul( b, a );

    ASSERT_TRUE( is_ok( get_error( result_ab ) ) );
    ASSERT_TRUE( is_ok( get_error( result_ba ) ) );
    EXPECT_TRUE( is_equal( result_ab, result_ba ) );
}

TEST_F( BigNumberMultiplicationTest, MultiplicationIsAssociative ) {
    double a_double = 12.0;
    double b_double = 34.0;
    double c_double = 56.0;
    BigNumber a = create_from_double( a_double );
    BigNumber b = create_from_double( b_double );
    BigNumber c = create_from_double( c_double );

    BigNumber ab = mul( a, b );
    BigNumber result_ab_c = mul( ab, c );

    BigNumber bc = mul( b, c );
    BigNumber result_a_bc = mul( a, bc );

    ASSERT_TRUE( is_ok( get_error( ab ) ) );
    ASSERT_TRUE( is_ok( get_error( bc ) ) );
    ASSERT_TRUE( is_ok( get_error( result_ab_c ) ) );
    ASSERT_TRUE( is_ok( get_error( result_a_bc ) ) );
    EXPECT_TRUE( is_equal( result_ab_c, result_a_bc ) );
}

TEST_F( BigNumberMultiplicationTest, MultiplyLargeNumbers ) {

    double a_double = 999999.0;
    double b_double = 888888.0;
    BigNumber a = create_from_double( a_double );
    BigNumber b = create_from_double( b_double );
    double expected_double = a_double * b_double;

    BigNumber result = mul( a, b );

    ASSERT_TRUE( is_ok( get_error( result ) ) );
    double actual_double = extract_to_double( result );
    EXPECT_TRUE(
        are_approximately_equal( actual_double, expected_double, 1e-6 ) );
}
