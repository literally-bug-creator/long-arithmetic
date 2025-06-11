#include <gtest/gtest.h>

#include "big_number.hpp"

using namespace big_number;

class BigNumberSubTest : public ::testing::Test {};

TEST_F( BigNumberSubTest, SubtractZeroFromNumber ) {
    BigNumber number = make_big_number( { 123 }, 2, false );
    BigNumber zero = make_big_number( { 0 }, 0, false );
    BigNumber expected_result = make_big_number( { 123 }, 2, false );

    BigNumber result = sub( number, zero );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberSubTest, SubtractNumberFromZero ) {
    BigNumber zero = make_big_number( { 0 }, 0, false );
    BigNumber number = make_big_number( { 456 }, 3, false );
    BigNumber expected_result = make_big_number( { 456 }, 3, true );

    BigNumber result = sub( zero, number );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberSubTest, SubtractNegativeNumberFromZero ) {
    BigNumber zero = make_big_number( { 0 }, 0, false );
    BigNumber negative = make_big_number( { 789 }, 1, true );
    BigNumber expected_result = make_big_number( { 789 }, 1, false );

    BigNumber result = sub( zero, negative );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberSubTest, SubtractPositiveNumbers ) {
    BigNumber num1 = make_big_number( { 500 }, 0, false );
    BigNumber num2 = make_big_number( { 300 }, 0, false );
    BigNumber expected_result = make_big_number( { 200 }, 0, false );

    BigNumber result = sub( num1, num2 );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberSubTest, SubtractNegativeNumbers ) {
    BigNumber num1 = make_big_number( { 200 }, 0, true );
    BigNumber num2 = make_big_number( { 100 }, 0, true );
    BigNumber expected_result = make_big_number( { 100 }, 0, true );

    BigNumber result = sub( num1, num2 );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberSubTest, SubtractNegativeFromPositive ) {
    BigNumber positive = make_big_number( { 100 }, 1, false );
    BigNumber negative = make_big_number( { 50 }, 1, true );
    BigNumber expected_result = make_big_number( { 150 }, 1, false );

    BigNumber result = sub( positive, negative );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberSubTest, SubtractPositiveFromNegative ) {
    BigNumber negative = make_big_number( { 100 }, 1, true );
    BigNumber positive = make_big_number( { 50 }, 1, false );
    BigNumber expected_result = make_big_number( { 150 }, 1, true );

    BigNumber result = sub( negative, positive );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberSubTest, SubtractSameNumber ) {
    BigNumber number = make_big_number( { 123, 456 }, 2, false );
    BigNumber expected_result = make_big_number( {}, 0, false );

    BigNumber result = sub( number, number );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberSubTest, SubtractSameNegativeNumber ) {
    BigNumber number = make_big_number( { 789 }, 1, true );
    BigNumber expected_result = make_big_number( {}, 0, false );

    BigNumber result = sub( number, number );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberSubTest, SubtractZeros ) {
    BigNumber zero1 = make_big_number( { 0 }, 0, false );
    BigNumber zero2 = make_big_number( { 0 }, 0, true );
    BigNumber expected_result = make_big_number( {}, 0, false );

    BigNumber result = sub( zero1, zero2 );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( BigNumberSubTest, SubtractAnticommutative ) {
    BigNumber num1 = make_big_number( { 100 }, 1, false );
    BigNumber num2 = make_big_number( { 200 }, 1, false );
    BigNumber expected_result_1 = make_big_number( { 100 }, 1, true );
    BigNumber expected_result_2 = make_big_number( { 100 }, 1, false );

    BigNumber result1 = sub( num1, num2 );
    BigNumber result2 = sub( num2, num1 );

    EXPECT_TRUE( is_equal( result1, expected_result_1 ) );
    EXPECT_TRUE( is_equal( result2, expected_result_2 ) );
}
