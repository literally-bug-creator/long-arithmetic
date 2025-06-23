#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "tools.hpp"

using namespace big_number;

class BigNumberNegTest : public ::testing::Test {
protected:
    Error error = get_default_error();
};

TEST_F( BigNumberNegTest, PositiveNumberBecomesNegative ) {
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );
    BigNumber expected = create_big_number( chunks, 0, true );

    auto result = neg( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberNegTest, NegativeNumberBecomesPositive ) {
    chunks chunks = { 456 };
    auto number = create_big_number( chunks, 0, true );
    BigNumber expected = create_big_number( chunks, 0, false );

    auto result = neg( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberNegTest, ZeroRemainsZero ) {
    BigNumber number = make_zero( error, false );
    BigNumber expected = make_zero( error, true );

    BigNumber result = neg( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberNegTest, NaNRemainsNaN ) {
    auto number = make_nan( error );
    BigNumber expected = make_nan( error, true );

    auto result = neg( number );

    EXPECT_FALSE( is_equal( result, expected ) );
}

TEST_F( BigNumberNegTest, PositiveInfinityRemainsPositiveInfinity ) {
    auto number = make_inf( error, false );
    BigNumber expected = make_inf( error, true );

    auto result = neg( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberNegTest, NegativeInfinityBecomesPositiveInfinity ) {
    auto number = make_inf( error, true );
    BigNumber expected = make_inf( error, false );

    auto result = neg( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberNegTest, NumberWithShiftRemainsUnchanged ) {
    chunks chunks = { 789 };
    auto number = create_big_number( chunks, 2, true );
    BigNumber expected = create_big_number( chunks, 2, false );

    auto result = neg( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}
