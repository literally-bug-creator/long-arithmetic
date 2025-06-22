#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "tools.hpp"

using namespace big_number;

class BigNumberAbsTest : public ::testing::Test {
protected:
    Error error = get_default_error();
};

TEST_F( BigNumberAbsTest, PositiveNumberRemainsUnchanged ) {
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );
    auto expected = create_big_number( chunks, 0, false );

    auto result = abs( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAbsTest, NegativeNumberBecomesPositive ) {
    chunks chunks = { 456 };
    auto number = create_big_number( chunks, 0, true );
    auto expected = create_big_number( chunks, 0, false );

    auto result = abs( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAbsTest, NumberWithShiftRemainsUnchanged ) {
    chunks chunks = { 789 };
    auto number = create_big_number( chunks, 2, true );
    auto expected = create_big_number( chunks, 2, false );

    auto result = abs( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAbsTest, ZeroRemainsZero ) {
    auto number = make_zero( error );
    auto expected = make_zero( error );

    auto result = abs( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAbsTest, NaNRemainsNaN ) {
    auto number = make_nan( error );
    auto expected = make_nan( error );

    auto result = abs( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAbsTest, PositiveInfinityRemainsPositiveInfinity ) {
    auto number = make_inf( error, false );
    auto expected = make_inf( error, false );

    auto result = abs( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberAbsTest, NegativeInfinityBecomesPositiveInfinity ) {
    auto number = make_inf( error, true );
    auto expected = make_inf( error, false );

    auto result = abs( number );

    EXPECT_TRUE( is_equal( result, expected ) );
}
