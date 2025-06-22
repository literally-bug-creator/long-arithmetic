#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "tools.hpp"

using namespace big_number;

class BigNumberAbsTest : public ::testing::Test {};

TEST_F( BigNumberAbsTest, PositiveNumberRemainsUnchanged ) {
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );

    auto result = abs( number );

    EXPECT_EQ( result.mantissa, chunks );
    EXPECT_EQ( result.shift, 0 );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::DEFAULT );
}

TEST_F( BigNumberAbsTest, NegativeNumberBecomesPositive ) {
    chunks chunks = { 456 };
    auto number = create_big_number( chunks, 0, true );

    auto result = abs( number );

    EXPECT_EQ( result.mantissa, chunks );
    EXPECT_EQ( result.shift, 0 );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::DEFAULT );
}

TEST_F( BigNumberAbsTest, ZeroRemainsZero ) {
    auto number = make_zero( Error{} );

    auto result = abs( number );

    EXPECT_EQ( result.mantissa, number.mantissa );
    EXPECT_EQ( result.shift, number.shift );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::ZERO );
}

TEST_F( BigNumberAbsTest, NaNRemainsNaN ) {
    auto number = make_nan( Error{} );

    auto result = abs( number );

    EXPECT_EQ( result.mantissa, number.mantissa );
    EXPECT_EQ( result.shift, number.shift );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::NOT_A_NUMBER );
}

TEST_F( BigNumberAbsTest, PositiveInfinityRemainsPositiveInfinity ) {
    auto number = make_inf( Error{}, false );

    auto result = abs( number );

    EXPECT_EQ( result.mantissa, number.mantissa );
    EXPECT_EQ( result.shift, number.shift );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::INF );
}

TEST_F( BigNumberAbsTest, NegativeInfinityBecomesPositiveInfinity ) {
    auto number = make_inf( Error{}, true );

    auto result = abs( number );

    EXPECT_EQ( result.mantissa, number.mantissa );
    EXPECT_EQ( result.shift, number.shift );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::INF );
}

TEST_F( BigNumberAbsTest, NumberWithShiftRemainsUnchanged ) {
    chunks chunks = { 789 };
    auto number = create_big_number( chunks, 2, true );

    auto result = abs( number );

    EXPECT_EQ( result.mantissa, chunks );
    EXPECT_EQ( result.shift, 2 );
    EXPECT_FALSE( result.is_negative );
    EXPECT_EQ( result.type, BigNumberType::DEFAULT );
}
