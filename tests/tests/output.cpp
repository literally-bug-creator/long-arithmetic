#include <gtest/gtest.h>
#include <vector>

#include "big_number.hpp"

using namespace big_number;

BigNumber create_test_number( const std::vector<chunk>& chunks,
                              int32_t exponent,
                              bool is_negative = false ) {
    BigNumber num;
    num.chunks = chunks;
    num.exponent = exponent;
    num.is_negative = is_negative;
    return num;
}

class ToStringTest : public ::testing::Test {};

TEST_F( ToStringTest, EmptyChunks ) {
    BigNumber num = create_test_number( {}, 0 );
    EXPECT_EQ( to_string( num ), "0" );
}

TEST_F( ToStringTest, PositiveIntegerZeroExponent ) {
    BigNumber num = create_test_number( { 123 }, 0 );
    EXPECT_EQ( to_string( num ), "123" );
}

TEST_F( ToStringTest, NegativeIntegerZeroExponent ) {
    BigNumber num = create_test_number( { 456 }, 0, true );
    EXPECT_EQ( to_string( num ), "-456" );
}

TEST_F( ToStringTest, PositiveNumberNegativeExponent ) {
    BigNumber num = create_test_number( { 789 }, -1 );
    EXPECT_EQ( to_string( num ), "789e-18" );
}

TEST_F( ToStringTest, NegativeNumberNegativeExponent ) {
    BigNumber num = create_test_number( { 321 }, -1, true );
    EXPECT_EQ( to_string( num ), "-321e-18" );
}

TEST_F( ToStringTest, PositiveNumberPositiveExponent ) {
    BigNumber num = create_test_number( { 654 }, 2 );
    EXPECT_EQ( to_string( num ), "654e36" );
}

TEST_F( ToStringTest, NegativeNumberPositiveExponent ) {
    BigNumber num = create_test_number( { 987 }, 3, true );
    EXPECT_EQ( to_string( num ), "-987e54" );
}

TEST_F( ToStringTest, MultipleChunksZeroExponent ) {
    BigNumber num = create_test_number( { 456, 123 }, 0 );
    EXPECT_EQ( to_string( num ), "123000000000000000456" );
}

TEST_F( ToStringTest, MultipleChunksPositiveExponent ) {
    BigNumber num = create_test_number( { 789, 456, 123 }, 1 );
    EXPECT_EQ( to_string( num ), "123000000000000000456000000000000000789e18" );
}

TEST_F( ToStringTest, MultipleChunksNegativeExponent ) {
    BigNumber num = create_test_number( { 111, 222 }, -2, true );
    EXPECT_EQ( to_string( num ), "-222000000000000000111e-36" );
}

TEST_F( ToStringTest, HighestChunkNoPadding ) {
    BigNumber num = create_test_number( { 999999999999999999ULL, 5 }, 0 );
    EXPECT_EQ( to_string( num ), "5999999999999999999" );
}

TEST_F( ToStringTest, LowerChunksWithPadding ) {
    BigNumber num = create_test_number( { 42, 7 }, 0 );
    EXPECT_EQ( to_string( num ), "7000000000000000042" );
}

TEST_F( ToStringTest, MaxChunkValue ) {
    BigNumber num = create_test_number( { 999999999999999999ULL }, -1 );
    EXPECT_EQ( to_string( num ), "999999999999999999e-18" );
}

TEST_F( ToStringTest, ExponentZeroVsNonZero ) {
    BigNumber num1 = create_test_number( { 100 }, 0 );
    BigNumber num2 = create_test_number( { 100 }, 1 );

    EXPECT_EQ( to_string( num1 ), "100" );    // без 'e'
    EXPECT_EQ( to_string( num2 ), "100e18" ); // с 'e'
}

TEST_F( ToStringTest, LargePositiveExponent ) {
    BigNumber num = create_test_number( { 1 }, 100 );
    EXPECT_EQ( to_string( num ), "1e1800" );
}

TEST_F( ToStringTest, LargeNegativeExponent ) {
    BigNumber num = create_test_number( { 1 }, -100, true );
    EXPECT_EQ( to_string( num ), "-1e-1800" );
}
