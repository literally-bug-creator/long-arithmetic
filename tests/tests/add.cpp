#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "tools.hpp"

using namespace big_number;

class BigNumberAddTest : public ::testing::Test {
protected:
    Error error = get_default_error();
};

TEST_F( BigNumberAddTest, AddTwoPositiveNumbers ) {
    BigNumber a = create_big_number( { 123 }, 0, false );
    BigNumber b = create_big_number( { 456 }, 0, false );
    BigNumber expected = create_big_number( { 579 }, 0, false );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddTwoNegativeNumbers ) {
    BigNumber a = create_big_number( { 123 }, 0, true );
    BigNumber b = create_big_number( { 456 }, 0, true );
    BigNumber expected = create_big_number( { 579 }, 0, true );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddWithCarry ) {
    BigNumber a = create_big_number( { 999999999999999999ULL }, 0, false );
    BigNumber b = create_big_number( { 1 }, 0, false );
    BigNumber expected = create_big_number( { 1 }, 1, false );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddWithDifferentShifts ) {
    BigNumber a = create_big_number( { 123 }, 0, false );
    BigNumber b = create_big_number( { 456 }, 2, false );
    BigNumber expected = create_big_number( { 123, 0, 456 }, 0, false );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddZeroToNumber ) {
    BigNumber zero = make_zero( error );
    BigNumber number = create_big_number( { 123 }, 0, false );

    BigNumber result = add( zero, number );

    EXPECT_TRUE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddNumberToZero ) {
    BigNumber number = create_big_number( { 123 }, 0, false );
    BigNumber zero = make_zero( error );

    BigNumber result = add( number, zero );

    EXPECT_TRUE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddTwoZeros ) {
    BigNumber zero1 = make_zero( error );
    BigNumber zero2 = make_zero( error );
    BigNumber expected = make_zero( error );

    BigNumber result = add( zero1, zero2 );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddPositiveAndNegative ) {
    BigNumber positive = create_big_number( { 500 }, 0, false );
    BigNumber negative = create_big_number( { 200 }, 0, true );
    BigNumber expected = create_big_number( { 300 }, 0, false );

    BigNumber result = add( positive, negative );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddNegativeAndPositive ) {
    BigNumber negative = create_big_number( { 500 }, 0, true );
    BigNumber positive = create_big_number( { 200 }, 0, false );
    BigNumber expected = create_big_number( { 300 }, 0, true );

    BigNumber result = add( negative, positive );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddMultipleChunks ) {
    BigNumber a = create_big_number( { 123, 456, 789 }, 0, false );
    BigNumber b = create_big_number( { 111, 222, 333 }, 0, false );
    BigNumber expected = create_big_number( { 234, 678, 1122 }, 0, false );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddDifferentChunkSizes ) {
    BigNumber a = create_big_number( { 123, 456 }, 0, false );
    BigNumber b = create_big_number( { 789 }, 0, false );
    BigNumber expected = create_big_number( { 912, 456 }, 0, false );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddNanToNumber ) {
    BigNumber nan = make_nan( error );
    BigNumber number = create_big_number( { 123 }, 0, false );

    BigNumber result = add( nan, number );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddNumberToNan ) {
    BigNumber number = create_big_number( { 123 }, 0, false );
    BigNumber nan = make_nan( error );

    BigNumber result = add( number, nan );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddPositiveInfinityToNumber ) {
    BigNumber inf = make_inf( error, false );
    BigNumber number = create_big_number( { 123 }, 0, false );

    BigNumber result = add( inf, number );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddNumberToPositiveInfinity ) {
    BigNumber number = create_big_number( { 123 }, 0, false );
    BigNumber inf = make_inf( error, false );

    BigNumber result = add( number, inf );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddNegativeInfinityToNumber ) {
    BigNumber inf = make_inf( error, true );
    BigNumber number = create_big_number( { 123 }, 0, false );

    BigNumber result = add( inf, number );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddTwoPositiveInfinities ) {
    BigNumber inf1 = make_inf( error, false );
    BigNumber inf2 = make_inf( error, false );
    BigNumber expected = make_inf( error, false );

    BigNumber result = add( inf1, inf2 );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddPositiveAndNegativeInfinity ) {
    BigNumber pos_inf = make_inf( error, false );
    BigNumber neg_inf = make_inf( error, true );

    BigNumber result = add( pos_inf, neg_inf );

    EXPECT_TRUE( result.type == BigNumberType::NOT_A_NUMBER );
}
