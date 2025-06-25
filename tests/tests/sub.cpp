#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "tools.hpp"

using namespace big_number;

class BigNumberSubTest : public ::testing::Test {};

TEST_F( BigNumberSubTest, SubtractTwoPositiveNumbersPositiveResult ) {
    auto a = create_big_number( { 500 }, 0, false );
    auto b = create_big_number( { 200 }, 0, false );
    auto expected = create_big_number( { 300 }, 0, false );

    auto result = sub( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractTwoPositiveNumbersNegativeResult ) {
    auto a = create_big_number( { 200 }, 0, false );
    auto b = create_big_number( { 500 }, 0, false );
    auto expected = create_big_number( { 300 }, 0, true );

    auto result = sub( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractTwoNegativeNumbers ) {
    auto a = create_big_number( { 500 }, 0, true );
    auto b = create_big_number( { 200 }, 0, true );
    auto expected = create_big_number( { 300 }, 0, true );

    auto result = sub( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractFromZero ) {
    auto zero = make_zero( Error{} );
    auto number = create_big_number( { 123 }, 0, false );
    auto expected = create_big_number( { 123 }, 0, true );

    auto result = sub( zero, number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractZero ) {
    auto number = create_big_number( { 123 }, 0, false );
    auto zero = make_zero( Error{} );

    auto result = sub( number, zero );

    EXPECT_TRUE( is_equal( result, number ) );
}

TEST_F( BigNumberSubTest, SubtractTwoZeros ) {
    auto zero1 = make_zero( Error{} );
    auto zero2 = make_zero( Error{} );
    auto expected = make_zero( Error{} );

    auto result = sub( zero1, zero2 );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractPositiveFromNegative ) {
    auto negative = create_big_number( { 200 }, 0, true );
    auto positive = create_big_number( { 300 }, 0, false );
    auto expected = create_big_number( { 500 }, 0, true );

    auto result = sub( negative, positive );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractNegativeFromPositive ) {
    auto positive = create_big_number( { 200 }, 0, false );
    auto negative = create_big_number( { 300 }, 0, true );
    auto expected = create_big_number( { 500 }, 0, false );

    auto result = sub( positive, negative );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractMultipleChunks ) {
    auto a = create_big_number( { 500, 600, 700 }, 0, false );
    auto b = create_big_number( { 100, 200, 300 }, 0, false );
    auto expected = create_big_number( { 400, 400, 400 }, 0, false );

    auto result = sub( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractWithDifferentShifts ) {
    auto a = create_big_number( { 456 }, 2, false );
    auto b = create_big_number( { 123 }, 0, false );
    auto expected = create_big_number(
        { 999999999999999877ULL, 999999999999999999ULL, 455 }, 0, false );

    auto result = sub( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractDifferentChunkSizes ) {
    auto a = create_big_number( { 500, 600 }, 0, false );
    auto b = create_big_number( { 100 }, 0, false );
    auto expected = create_big_number( { 400, 600 }, 0, false );

    auto result = sub( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractEqualNumbers ) {
    auto a = create_big_number( { 123, 456 }, 0, false );
    auto b = create_big_number( { 123, 456 }, 0, false );

    auto result = sub( a, b );

    auto expected = make_zero( Error{} );
    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberSubTest, SubtractNumberFromNan ) {
    auto nan = make_nan( Error{} );
    auto number = create_big_number( { 123 }, 0, false );

    auto result = sub( nan, number );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberSubTest, SubtractNanFromNumber ) {
    auto number = create_big_number( { 123 }, 0, false );
    auto nan = make_nan( Error{} );

    auto result = sub( number, nan );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberSubTest, SubtractNumberFromPositiveInfinity ) {
    auto inf = make_inf( Error{}, false );
    auto number = create_big_number( { 123 }, 0, false );

    auto result = sub( inf, number );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberSubTest, SubtractPositiveInfinityFromNumber ) {
    auto number = create_big_number( { 123 }, 0, false );
    auto inf = make_inf( Error{}, false );

    auto result = sub( number, inf );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberSubTest, SubtractNegativeInfinityFromNumber ) {
    auto number = create_big_number( { 123 }, 0, false );
    auto inf = make_inf( Error{}, true );

    auto result = sub( number, inf );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberSubTest, SubtractTwoPositiveInfinities ) {
    auto inf1 = make_inf( Error{}, false );
    auto inf2 = make_inf( Error{}, false );

    auto result = sub( inf1, inf2 );

    EXPECT_FALSE( is_equal( result, inf1 ) );
}

TEST_F( BigNumberSubTest, SubtractNegativeInfinityFromPositiveInfinity ) {
    auto pos_inf = make_inf( Error{}, false );
    auto neg_inf = make_inf( Error{}, true );

    auto result = sub( pos_inf, neg_inf );

    EXPECT_FALSE( is_equal( result, pos_inf ) );
    EXPECT_FALSE( is_equal( result, neg_inf ) );
}

TEST_F( BigNumberSubTest, SubtractPreservesErrorFromLeftOperand ) {
    auto number = create_big_number( { 456 }, 0, false );
    auto error_number = make_nan( Error{} );

    auto result = sub( error_number, number );

    EXPECT_FALSE( is_equal( result, number ) );
}
