#include <gtest/gtest.h>

#include "big_number.hpp"
#include "error.hpp"

using namespace big_number;

class BigNumberAddTest : public ::testing::Test {};

TEST_F( BigNumberAddTest, AddZeroToNumber ) {
    BigNumber zero = make_big_number( { 0 }, 0, false );
    BigNumber number = make_big_number( { 123 }, 2, false );

    BigNumber result = add( zero, number );

    EXPECT_TRUE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddNumberToZero ) {
    BigNumber zero = make_big_number( { 0 }, 0, false );
    BigNumber number = make_big_number( { 456 }, 3, true );

    BigNumber result = add( number, zero );

    EXPECT_TRUE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddPositiveNumbers ) {
    BigNumber num1 = make_big_number( { 100 }, 0, false );
    BigNumber num2 = make_big_number( { 200 }, 0, false );
    BigNumber expected_result = make_big_number( { 300 }, 0, false );

    BigNumber result = add( num1, num2 );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberAddTest, AddNegativeNumbers ) {
    BigNumber num1 = make_big_number( { 100 }, 0, true );
    BigNumber num2 = make_big_number( { 200 }, 0, true );
    BigNumber expected_result = make_big_number( { 300 }, 0, true );

    BigNumber result = add( num1, num2 );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberAddTest, AddPositiveAndNegative ) {
    BigNumber positive = make_big_number( { 300 }, 1, false );
    BigNumber negative = make_big_number( { 200 }, 1, true );

    BigNumber result = add( positive, negative );

    EXPECT_EQ( get_error_code( get_error( result ) ),
               get_error_code( get_default_error() ) );
}

TEST_F( BigNumberAddTest, AddNegativeAndPositive ) {
    BigNumber negative = make_big_number( { 150 }, 2, true );
    BigNumber positive = make_big_number( { 100 }, 2, false );

    BigNumber result = add( negative, positive );

    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, AddCommutative ) {
    BigNumber num1 = make_big_number( { 123 }, 1, false );
    BigNumber num2 = make_big_number( { 456 }, 1, false );

    BigNumber result1 = add( num1, num2 );
    BigNumber result2 = add( num2, num1 );

    EXPECT_TRUE( is_equal( result1, result2 ) );
}

TEST_F( BigNumberAddTest, AddZeros ) {
    BigNumber zero1 = make_big_number( {}, 0, false );
    BigNumber zero2 = make_big_number( {}, 0, true );
    BigNumber expected_result = make_big_number( {}, 0, false );

    BigNumber result = add( zero1, zero2 );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}
