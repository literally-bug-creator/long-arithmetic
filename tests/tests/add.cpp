#include <gtest/gtest.h>

#include "big_number.hpp"

using namespace big_number;

class BigNumberAddTest : public ::testing::Test {};

TEST_F( BigNumberAddTest, PositiveNumbersBasic ) {
    BigNumber a = make_big_number( { 1, 2, 3 }, 0 );
    BigNumber b = make_big_number( { 4, 5, 6 }, 0 );
    BigNumber expected = make_big_number( { 5, 7, 9 }, 0 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, WithCarry ) {
    BigNumber a = make_big_number( { 9, 9, 9 }, 0 );
    BigNumber b = make_big_number( { 1 }, 0 );
    BigNumber expected = make_big_number( { 1, 0, 0, 0 }, 0 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, AddWithZero ) {
    BigNumber a = make_big_number( { 1, 2, 3 }, 0 );
    BigNumber zero = make_big_number( { 0 }, 0 );

    BigNumber result = add( a, zero );

    EXPECT_TRUE( is_equal( result, a ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, NegativeNumbers ) {
    BigNumber a = make_big_number( { 1, 2, 3 }, 0, true );
    BigNumber b = make_big_number( { 4, 5, 6 }, 0, true );
    BigNumber expected = make_big_number( { 5, 7, 9 }, 0, true );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, DifferentSignsPositiveResult ) {
    BigNumber a = make_big_number( { 5, 0, 0 }, 0 );
    BigNumber b = make_big_number( { 2, 0, 0 }, 0, true );
    BigNumber expected = make_big_number( { 3, 0, 0 }, 0 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, DifferentSignsNegativeResult ) {
    BigNumber a = make_big_number( { 2, 0, 0 }, 0 );
    BigNumber b = make_big_number( { 5, 0, 0 }, 0, true );
    BigNumber expected = make_big_number( { 3, 0, 0 }, 0, true );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, DifferentSignsZeroResult ) {
    BigNumber a = make_big_number( { 1, 2, 3 }, 0 );
    BigNumber b = make_big_number( { 1, 2, 3 }, 0, true );
    BigNumber expected = make_big_number( { 0 }, 0 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, DifferentExponents ) {
    BigNumber a = make_big_number( { 1, 2, 3 }, 2 );
    BigNumber b = make_big_number( { 4, 5, 6 }, 0 );
    BigNumber expected = make_big_number( { 1, 2, 7, 5, 6 }, 0 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, DifferentLengths ) {
    BigNumber a = make_big_number( { 9, 9, 9, 9, 9, 9 }, 0 );
    BigNumber b = make_big_number( { 1 }, 0 );
    BigNumber expected = make_big_number( { 1, 0, 0, 0, 0, 0, 0 }, 0 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, VeryLargeNumbers ) {
    BigNumber a = make_big_number( { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
                                     9, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8,
                                     8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },
                                   0 );
    BigNumber b = make_big_number( { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                                   0 );
    BigNumber zero = make_big_number( { 0 }, 0 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_ok( get_error( result ) ) );
    EXPECT_FALSE( is_equal( result, zero ) );
    EXPECT_FALSE( is_lower_than( result, a ) );
    EXPECT_FALSE( is_lower_than( result, b ) );
}

TEST_F( BigNumberAddTest, MaxChunkValues ) {
    BigNumber a = make_big_number(
        { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 }, 0 );
    BigNumber b = make_big_number(
        { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 }, 0 );
    BigNumber zero = make_big_number( { 0 }, 0 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_ok( get_error( result ) ) );
    EXPECT_FALSE( is_equal( result, zero ) );
    EXPECT_FALSE( is_lower_than( result, a ) );
    EXPECT_FALSE( is_lower_than( result, b ) );
}

TEST_F( BigNumberAddTest, BorrowingRequired ) {
    BigNumber a = make_big_number( { 1, 0, 0, 0 }, 0 );
    BigNumber b = make_big_number( { 1 }, 0, true );
    BigNumber expected = make_big_number( { 9, 9, 9 }, 0 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, FractionalNumbers ) {
    BigNumber a = make_big_number( { 1, 5 }, -1 );
    BigNumber b = make_big_number( { 2, 5 }, -1 );
    BigNumber expected = make_big_number( { 4, 0 }, -1 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
    EXPECT_TRUE( is_ok( get_error( result ) ) );
}

TEST_F( BigNumberAddTest, ResultNormalization ) {
    BigNumber a = make_big_number( { 9, 9, 9 }, 0 );
    BigNumber b = make_big_number( { 1 }, 0 );
    BigNumber expected = make_big_number( { 1, 0, 0, 0 }, 0 );

    BigNumber result = add( a, b );

    EXPECT_TRUE( is_ok( get_error( result ) ) );
    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAddTest, Commutativity ) {
    BigNumber a = make_big_number( { 1, 2, 3 }, 1 );
    BigNumber b = make_big_number( { 4, 5, 6 }, -1 );

    BigNumber result1 = add( a, b );
    BigNumber result2 = add( b, a );

    EXPECT_TRUE( is_equal( result1, result2 ) );
    EXPECT_TRUE( is_ok( get_error( result1 ) ) );
    EXPECT_TRUE( is_ok( get_error( result2 ) ) );
}
