#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "tools.hpp"

using namespace big_number;

class BigNumberNegTest : public ::testing::Test {};

TEST_F( BigNumberNegTest, PositiveNumberBecomesNegative ) {
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );

    auto result = neg( number );

    EXPECT_EQ( result.mantissa, chunks );
    EXPECT_EQ( result.shift, 0 );
    EXPECT_TRUE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::DEFAULT );
}

TEST_F( BigNumberNegTest, NegativeNumberBecomesPositive ) {
    chunks chunks = { 456 };
    auto number = create_big_number( chunks, 0, true );

    auto result = neg( number );

    EXPECT_EQ( result.mantissa, chunks );
    EXPECT_EQ( result.shift, 0 );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::DEFAULT );
}

TEST_F( BigNumberNegTest, ZeroRemainsZero ) {
    auto number = make_zero( Error{} );

    auto result = neg( number );

    EXPECT_EQ( result.mantissa, number.mantissa );
    EXPECT_EQ( result.shift, number.shift );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::ZERO );
}

TEST_F( BigNumberNegTest, NaNRemainsNaN ) {
    auto number = make_nan( Error{} );

    auto result = neg( number );

    EXPECT_EQ( result.mantissa, number.mantissa );
    EXPECT_EQ( result.shift, number.shift );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::NOT_A_NUMBER );
}

TEST_F( BigNumberNegTest, PositiveInfinityRemainsPositiveInfinity ) {
    auto number = make_inf( Error{}, false );

    auto result = neg( number );

    EXPECT_EQ( result.mantissa, number.mantissa );
    EXPECT_EQ( result.shift, number.shift );
    EXPECT_TRUE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::INF );
}

TEST_F( BigNumberNegTest, NegativeInfinityBecomesPositiveInfinity ) {
    auto number = make_inf( Error{}, true );

    auto result = neg( number );

    EXPECT_EQ( result.mantissa, number.mantissa );
    EXPECT_EQ( result.shift, number.shift );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::INF );
}

TEST_F( BigNumberNegTest, NumberWithShiftRemainsUnchanged ) {
    chunks chunks = { 789 };
    auto number = create_big_number( chunks, 2, true );

    auto result = neg( number );

    EXPECT_EQ( result.mantissa, chunks );
    EXPECT_EQ( result.shift, 2 );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::DEFAULT );
}
