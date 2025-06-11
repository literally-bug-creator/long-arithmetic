#include <gtest/gtest.h>

#include "big_number.hpp"

using namespace big_number;

class BigNumberAbsNegTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F( BigNumberAbsNegTest, AbsPositiveNumber ) {
    BigNumber positive = make_big_number( { 123, 456 }, 5, false );
    BigNumber result = abs( positive );

    EXPECT_EQ( result.chunks, positive.chunks );
    EXPECT_EQ( result.exponent, positive.exponent );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( BigNumberAbsNegTest, AbsNegativeNumber ) {
    BigNumber negative = make_big_number( { 789, 12 }, 3, true );
    BigNumber result = abs( negative );

    EXPECT_EQ( result.chunks, negative.chunks );
    EXPECT_EQ( result.exponent, negative.exponent );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( BigNumberAbsNegTest, AbsZero ) {
    BigNumber zero = make_big_number( { 0 }, 0, true );
    BigNumber result = abs( zero );

    EXPECT_FALSE( result.is_negative );
}

TEST_F( BigNumberAbsNegTest, NegPositiveNumber ) {
    BigNumber positive = make_big_number( { 555, 666 }, 7, false );
    BigNumber result = neg( positive );

    EXPECT_EQ( result.chunks, positive.chunks );
    EXPECT_EQ( result.exponent, positive.exponent );
    EXPECT_TRUE( result.is_negative );
}

TEST_F( BigNumberAbsNegTest, NegNegativeNumber ) {
    BigNumber negative = make_big_number( { 777, 888 }, 4, true );
    BigNumber result = neg( negative );

    EXPECT_EQ( result.chunks, negative.chunks );
    EXPECT_EQ( result.exponent, negative.exponent );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( BigNumberAbsNegTest, DoubleNegation ) {
    BigNumber original = make_big_number( { 123, 456 }, 2, false );
    BigNumber doubleNeg = neg( neg( original ) );

    EXPECT_EQ( doubleNeg.chunks, original.chunks );
    EXPECT_EQ( doubleNeg.exponent, original.exponent );
    EXPECT_EQ( doubleNeg.is_negative, original.is_negative );
}

TEST_F( BigNumberAbsNegTest, AbsOfNegation ) {
    BigNumber original = make_big_number( { 987, 654 }, 3, false );
    BigNumber absNeg = abs( neg( original ) );
    BigNumber absOrig = abs( original );

    EXPECT_TRUE( is_equal( absNeg, absOrig ) );
}
