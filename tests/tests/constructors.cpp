#include <gtest/gtest.h>

#include "big_number.hpp"

using namespace big_number;

class ConstructorTest : public ::testing::Test {};

typedef std::vector<chunk> chunks;
typedef std::vector<int> digits;

TEST_F( ConstructorTest, SingleDigit ) {
    chunks expected_chunks = { 3 };
    int32_t expected_exp = -1;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 3 }, -18 );

    EXPECT_EQ( a.chunks, expected_chunks );
    EXPECT_EQ( a.exponent, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( ConstructorTest, NegativeNumber ) {
    chunks expected_chunks = { 123 };
    int32_t expected_exp = 0;
    bool expected_sign = true;

    BigNumber a = make_big_number( { 1, 2, 3 }, 0, true );

    EXPECT_EQ( a.chunks, expected_chunks );
    EXPECT_EQ( a.exponent, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( ConstructorTest, OneFullChunk ) {
    chunks expected_chunks = { 123456789012345678 };
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number(
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 }, 0 );

    EXPECT_EQ( a.chunks, expected_chunks );
    EXPECT_EQ( a.exponent, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( ConstructorTest, TwoChunks ) {
    chunks expected_chunks = { 123456789012345678, 456 };
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number(
        { 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 }, 0 );

    EXPECT_EQ( a.chunks, expected_chunks );
    EXPECT_EQ( a.exponent, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( ConstructorTest, Zero ) {
    chunks expected_chunks = {};
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 0 }, 0 );

    EXPECT_EQ( a.chunks, expected_chunks );
    EXPECT_EQ( a.exponent, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( ConstructorTest, NonDivisibleExponent ) {
    chunks expected_chunks = { 1230000000000000 };
    int32_t expected_exp = -1;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 1, 2, 3 }, -5 );

    EXPECT_EQ( a.chunks, expected_chunks );
    EXPECT_EQ( a.exponent, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( ConstructorTest, LargePositiveExponent ) {
    chunks expected_chunks = { 123 };
    int32_t expected_exp = 2;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 1, 2, 3 }, 36 );

    EXPECT_EQ( a.chunks, expected_chunks );
    EXPECT_EQ( a.exponent, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( ConstructorTest, LargeNegativeExponent ) {
    chunks expected_chunks = { 123 };
    int32_t expected_exp = -2;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 1, 2, 3 }, -36 );

    EXPECT_EQ( a.chunks, expected_chunks );
    EXPECT_EQ( a.exponent, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}
