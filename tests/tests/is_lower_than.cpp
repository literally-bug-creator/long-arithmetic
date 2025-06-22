#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "tools.hpp"

using namespace big_number;

class BigNumberIsLowerThanTest : public ::testing::Test {};

TEST_F( BigNumberIsLowerThanTest, PositiveNumberVsNegativeNumber ) {
    chunks chunks1 = { 123 };
    chunks chunks2 = { 456 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, true );

    EXPECT_FALSE( is_lower_than( number1, number2 ) );
    EXPECT_TRUE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, PositiveNumberVsZero ) {
    chunks chunks = { 123 };
    auto number1 = create_big_number( chunks, 0, false );
    auto number2 = make_zero( Error{} );

    EXPECT_FALSE( is_lower_than( number1, number2 ) );
    EXPECT_TRUE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, NegativeNumberVsZero ) {
    chunks chunks = { 123 };
    auto number1 = create_big_number( chunks, 0, true );
    auto number2 = make_zero( Error{} );

    EXPECT_FALSE( is_lower_than( number2, number1 ) );
    EXPECT_TRUE( is_lower_than( number1, number2 ) );
}

TEST_F( BigNumberIsLowerThanTest, PositiveNumberVsPositiveInfinity ) {
    chunks chunks = { 123 };
    auto number1 = create_big_number( chunks, 0, false );
    auto number2 = make_inf( Error{}, false );

    EXPECT_TRUE( is_lower_than( number1, number2 ) );
    EXPECT_FALSE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, NegativeNumberVsNegativeInfinity ) {
    chunks chunks = { 123 };
    auto number1 = create_big_number( chunks, 0, true );
    auto number2 = make_inf( Error{}, true );

    EXPECT_FALSE( is_lower_than( number1, number2 ) );
    EXPECT_TRUE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, PositiveNumberVsNaN ) {
    chunks chunks = { 123 };
    auto number1 = create_big_number( chunks, 0, false );
    auto number2 = make_nan( Error{} );

    EXPECT_FALSE( is_lower_than( number1, number2 ) );
    EXPECT_FALSE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, NegativeNumberVsNaN ) {
    chunks chunks = { 123 };
    auto number1 = create_big_number( chunks, 0, true );
    auto number2 = make_nan( Error{} );

    EXPECT_FALSE( is_lower_than( number1, number2 ) );
    EXPECT_FALSE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, PositiveInfinityVsNegativeInfinity ) {
    auto number1 = make_inf( Error{}, false );
    auto number2 = make_inf( Error{}, true );

    EXPECT_FALSE( is_lower_than( number1, number2 ) );
    EXPECT_TRUE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, PositiveInfinityVsNaN ) {
    auto number1 = make_inf( Error{}, false );
    auto number2 = make_nan( Error{} );

    EXPECT_FALSE( is_lower_than( number1, number2 ) );
    EXPECT_FALSE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, NegativeInfinityVsNaN ) {
    auto number1 = make_inf( Error{}, true );
    auto number2 = make_nan( Error{} );

    EXPECT_FALSE( is_lower_than( number1, number2 ) );
    EXPECT_FALSE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, PositiveNumberVsPositiveNumber ) {
    chunks chunks1 = { 123 };
    chunks chunks2 = { 456 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, false );

    EXPECT_TRUE( is_lower_than( number1, number2 ) );
    EXPECT_FALSE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, NegativeNumberVsNegativeNumber ) {
    chunks chunks1 = { 123 };
    chunks chunks2 = { 456 };
    auto number1 = create_big_number( chunks1, 0, true );
    auto number2 = create_big_number( chunks2, 0, true );

    EXPECT_FALSE( is_lower_than( number1, number2 ) );
    EXPECT_TRUE( is_lower_than( number2, number1 ) );
}

TEST_F( BigNumberIsLowerThanTest, DifferentChunkSizes ) {
    chunks chunks1 = { 123, 456 };
    chunks chunks2 = { 123 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, false );

    EXPECT_TRUE( is_lower_than( number2, number1 ) );
    EXPECT_FALSE( is_lower_than( number1, number2 ) );
}

TEST_F( BigNumberIsLowerThanTest, DifferentChunkSizesAndShifts ) {
    chunks chunks1 = { 123, 456 };
    chunks chunks2 = { 123 };
    auto number1 = create_big_number( chunks1, 1, false );
    auto number2 = create_big_number( chunks2, 0, false );

    EXPECT_TRUE( is_lower_than( number2, number1 ) );
    EXPECT_FALSE( is_lower_than( number1, number2 ) );
}

TEST_F( BigNumberIsLowerThanTest, NaNVssNaN ) {
    auto number1 = make_nan( Error{} );
    auto number2 = make_nan( Error{} );

    EXPECT_FALSE( is_lower_than( number1, number2 ) );
    EXPECT_FALSE( is_lower_than( number2, number1 ) );
}
