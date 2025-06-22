#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "error.hpp"

using namespace big_number;

class BigNumberIsEqualTest : public ::testing::Test {
protected:
    BigNumber create_big_number( const chunks& chunks,
                                 int32_t shift,
                                 bool is_negative = false,
                                 BigNumberType type = BigNumberType::DEFAULT ) {
        BigNumber number;
        number.mantissa = chunks;
        number.shift = shift;
        number.is_negative = is_negative;
        number.type = type;
        number.error = Error{};
        return number;
    }
};

TEST_F( BigNumberIsEqualTest, EqualNanAndNan ) {
    auto number1 = make_nan( get_default_error() );
    auto number2 = make_nan( get_default_error() );

    EXPECT_FALSE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, EqualNegativeInfAndNan ) {
    auto number1 = make_inf( get_default_error(), true );
    auto number2 = make_nan( get_default_error() );

    EXPECT_FALSE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, EqualPositiveInfAndNan ) {
    auto number1 = make_inf( get_default_error(), false );
    auto number2 = make_nan( get_default_error() );

    EXPECT_FALSE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, EqualNumberAndNan ) {
    chunks mantissa = { 9999999 };
    int32_t shift = 0;
    auto neg_number = create_big_number( mantissa, shift, true );
    auto pos_number = create_big_number( mantissa, shift, false );
    auto nan = make_nan( get_default_error() );

    EXPECT_FALSE( is_equal( neg_number, nan ) );
    EXPECT_FALSE( is_equal( pos_number, nan ) );
}

TEST_F( BigNumberIsEqualTest, EqualPositiveNumbers ) {
    chunks chunks = { 123 };
    auto number1 = create_big_number( chunks, 0, false );
    auto number2 = create_big_number( chunks, 0, false );

    EXPECT_TRUE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, EqualNegativeNumbers ) {
    chunks chunks = { 456 };
    auto number1 = create_big_number( chunks, 0, true );
    auto number2 = create_big_number( chunks, 0, true );

    EXPECT_TRUE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, EqualZeroNumbers ) {
    auto number1 = make_zero( Error{} );
    auto number2 = make_zero( Error{} );

    EXPECT_TRUE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, EqualPositiveInfinityNumbers ) {
    auto number1 = make_inf( Error{}, false );
    auto number2 = make_inf( Error{}, false );

    EXPECT_TRUE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, EqualNegativeInfinityNumbers ) {
    auto number1 = make_inf( Error{}, true );
    auto number2 = make_inf( Error{}, true );

    EXPECT_TRUE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, DifferentMantissa ) {
    chunks chunks1 = { 123 };
    chunks chunks2 = { 456 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, false );

    EXPECT_FALSE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, DifferentShift ) {
    chunks chunks = { 123 };
    auto number1 = create_big_number( chunks, 0, false );
    auto number2 = create_big_number( chunks, 1, false );

    EXPECT_FALSE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, DifferentSign ) {
    chunks chunks = { 123 };
    auto number1 = create_big_number( chunks, 0, false );
    auto number2 = create_big_number( chunks, 0, true );

    EXPECT_FALSE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, DifferentType ) {
    chunks chunks = { 123 };
    auto number1 =
        create_big_number( chunks, 0, false, BigNumberType::DEFAULT );
    auto number2 = create_big_number( chunks, 0, false, BigNumberType::INF );

    EXPECT_FALSE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, DifferentChunkSizes ) {
    chunks chunks1 = { 123, 456 };
    chunks chunks2 = { 123 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, false );

    EXPECT_FALSE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, DifferentChunkContents ) {
    chunks chunks1 = { 123, 456 };
    chunks chunks2 = { 123, 789 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, false );

    EXPECT_FALSE( is_equal( number1, number2 ) );
}

TEST_F( BigNumberIsEqualTest, EqualPositiveAndNegativeInfinity ) {
    auto number1 = make_inf( Error{}, false );
    auto number2 = make_inf( Error{}, true );

    EXPECT_FALSE( is_equal( number1, number2 ) );
}
