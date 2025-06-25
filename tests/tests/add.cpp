#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "tools.hpp"

using namespace big_number;

class BigNumberAddTest : public ::testing::Test {};

TEST_F( BigNumberAddTest, AddTwoPositiveNumbers ) {
    auto a = create_big_number( { 123 }, 0, false );
    auto b = create_big_number( { 456 }, 0, false );
    auto expected = create_big_number( { 579 }, 0, false );

    auto result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddTwoNegativeNumbers ) {
    auto a = create_big_number( { 123 }, 0, true );
    auto b = create_big_number( { 456 }, 0, true );
    auto expected = create_big_number( { 579 }, 0, true );

    auto result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddWithCarry ) {
    auto a = create_big_number( { 999999999999999999ULL }, 0, false );
    auto b = create_big_number( { 1 }, 0, false );
    auto expected = create_big_number( { 1 }, 1, false );

    auto result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

// Test addition with different shifts
TEST_F( BigNumberAddTest, AddWithDifferentShifts ) {
    auto a = create_big_number( { 123 }, 0, false );
    auto b = create_big_number( { 456 }, 2, false );
    auto expected = create_big_number( { 123, 0, 456 }, 0, false );

    auto result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddZeroToNumber ) {
    auto zero = make_zero( Error{} );
    auto number = create_big_number( { 123 }, 0, false );

    auto result = add( zero, number );

    EXPECT_TRUE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddNumberToZero ) {
    auto number = create_big_number( { 123 }, 0, false );
    auto zero = make_zero( Error{} );

    auto result = add( number, zero );

    EXPECT_TRUE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddTwoZeros ) {
    auto zero1 = make_zero( Error{} );
    auto zero2 = make_zero( Error{} );
    auto expected = make_zero( Error{} );

    auto result = add( zero1, zero2 );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddPositiveAndNegative ) {
    auto positive = create_big_number( { 500 }, 0, false );
    auto negative = create_big_number( { 200 }, 0, true );
    auto expected = create_big_number( { 300 }, 0, false );

    auto result = add( positive, negative );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddNegativeAndPositive ) {
    auto negative = create_big_number( { 500 }, 0, true );
    auto positive = create_big_number( { 200 }, 0, false );
    auto expected = create_big_number( { 300 }, 0, true );

    auto result = add( negative, positive );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddMultipleChunks ) {
    auto a = create_big_number( { 123, 456, 789 }, 0, false );
    auto b = create_big_number( { 111, 222, 333 }, 0, false );
    auto expected = create_big_number( { 234, 678, 1122 }, 0, false );

    auto result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddDifferentChunkSizes ) {
    auto a = create_big_number( { 123, 456 }, 0, false );
    auto b = create_big_number( { 789 }, 0, false );
    auto expected = create_big_number( { 912, 456 }, 0, false );

    auto result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddNanToNumber ) {
    auto nan = make_nan( Error{} );
    auto number = create_big_number( { 123 }, 0, false );

    auto result = add( nan, number );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddNumberToNan ) {
    auto number = create_big_number( { 123 }, 0, false );
    auto nan = make_nan( Error{} );

    auto result = add( number, nan );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddPositiveInfinityToNumber ) {
    auto inf = make_inf( Error{}, false );
    auto number = create_big_number( { 123 }, 0, false );

    auto result = add( inf, number );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddNumberToPositiveInfinity ) {
    auto number = create_big_number( { 123 }, 0, false );
    auto inf = make_inf( Error{}, false );

    auto result = add( number, inf );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddNegativeInfinityToNumber ) {
    auto inf = make_inf( Error{}, true );
    auto number = create_big_number( { 123 }, 0, false );

    auto result = add( inf, number );

    EXPECT_FALSE( is_equal( result, number ) );
}

TEST_F( BigNumberAddTest, AddTwoPositiveInfinities ) {
    auto inf1 = make_inf( Error{}, false );
    auto inf2 = make_inf( Error{}, false );

    auto result = add( inf1, inf2 );

    EXPECT_FALSE( is_equal( result, inf1 ) );
}

TEST_F( BigNumberAddTest, AddPositiveAndNegativeInfinity ) {
    auto pos_inf = make_inf( Error{}, false );
    auto neg_inf = make_inf( Error{}, true );

    auto result = add( pos_inf, neg_inf );

    EXPECT_FALSE( is_equal( result, pos_inf ) );
    EXPECT_FALSE( is_equal( result, neg_inf ) );
}

TEST_F( BigNumberAddTest, AddLargeNumbers ) {
    chunks large_chunks( 10, ALMOST_MAX_CHUNK );
    auto a = create_big_number( large_chunks, 0, false );
    auto b = create_big_number( large_chunks, 0, false );
    auto expected = make_zero( Error{} );

    auto result = add( a, b );

    EXPECT_FALSE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, AddPreservesErrorFromLeftOperand ) {
    auto number = create_big_number( { 456 }, 0, false );
    auto error_number = make_nan( Error{} );

    auto result = add( error_number, number );

    EXPECT_FALSE( is_equal( result, number ) );
}
