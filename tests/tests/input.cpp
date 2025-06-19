#include <gtest/gtest.h>

#include "big_number.hpp"
#include "error.hpp"

using namespace big_number;

class InputTest : public ::testing::Test {};

TEST_F( InputTest, SingleDigit ) {
    chunks expected_chunks = { 3 };
    int32_t expected_exp = -1;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 3 }, -18, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, NegativeNumber ) {
    chunks expected_chunks = { 123 };
    int32_t expected_exp = 0;
    bool expected_sign = true;

    BigNumber a = make_big_number( { 1, 2, 3 }, 0, true, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, OneFullChunk ) {
    chunks expected_chunks = { 123456789012345678 };
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number(
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 },
        0,
        false,
        get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, TwoChunks ) {
    chunks expected_chunks = { 123456789012345678, 456 };
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number(
        { 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 },
        0,
        false,
        get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, Zero ) {
    chunks expected_chunks = {};
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 0 }, 0, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, NonDivisibleShift ) {
    chunks expected_chunks = { 1230000000000000 };
    int32_t expected_exp = -1;
    bool expected_sign = false;

    BigNumber a =
        make_big_number( { 1, 2, 3 }, -5, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, LargePositiveshift ) {
    chunks expected_chunks = { 123 };
    int32_t expected_exp = 2;
    bool expected_sign = false;

    BigNumber a =
        make_big_number( { 1, 2, 3 }, 36, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, LargeNegativeshift ) {
    chunks expected_chunks = { 123 };
    int32_t expected_exp = -2;
    bool expected_sign = false;

    BigNumber a =
        make_big_number( { 1, 2, 3 }, -36, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, EmptyDigits ) {
    chunks expected_chunks = {};
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number( {}, 0, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, LeadingZerosInDigits ) {
    chunks expected_chunks = {};
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a =
        make_big_number( { 0, 0, 1, 2, 3 }, 0, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, TrailingZerosInDigits ) {
    chunks expected_chunks = { 456, 123 };
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number(
        { 1, 2, 3, 4, 5, 6, 0, 0, 0 }, 0, false, get_default_error() );

    EXPECT_NE( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, ZeroWithPositiveShift ) {
    chunks expected_chunks = {};
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 0 }, 3, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, ZeroWithNegativeShift ) {
    chunks expected_chunks = {};
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 0 }, -2, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, NegativeExponentNoPadding ) {
    chunks expected_chunks = { 300000000000000000, 12 };
    int32_t expected_exp = -1;
    bool expected_sign = false;

    BigNumber a =
        make_big_number( { 1, 2, 3 }, -1, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, NegativeExponentWithPadding ) {
    chunks expected_chunks = { 1230 };
    int32_t expected_exp = -1;
    bool expected_sign = false;

    BigNumber a =
        make_big_number( { 1, 2, 3 }, -17, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, NegativeShiftOverflow ) {
    Error error = get_default_error();
    BigNumber expected = make_zero( error );

    BigNumber a =
        make_big_number( { 1 }, -( 100000 + ( 2 * BASE ) ), false, error );

    EXPECT_EQ( a.mantissa, expected.mantissa );
    EXPECT_EQ( a.shift, expected.shift );
    EXPECT_EQ( a.is_negative, expected.is_negative );
    EXPECT_EQ( a.type, expected.type );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, PositiveShiftOverflow ) {
    Error error = get_default_error();
    BigNumber expected = make_inf( error, false );

    BigNumber a = make_big_number( { 1 }, 100000 + ( 2 * BASE ), false, error );

    EXPECT_EQ( a.mantissa, expected.mantissa );
    EXPECT_EQ( a.shift, expected.shift );
    EXPECT_EQ( a.is_negative, expected.is_negative );
    EXPECT_EQ( a.type, expected.type );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, MantissaUnderflow ) {
    Error error = get_default_error();
    BigNumber expected = make_zero( error );

    BigNumber a = make_big_number( { 0 }, 0, false, error );

    EXPECT_EQ( a.mantissa, expected.mantissa );
    EXPECT_EQ( a.shift, expected.shift );
    EXPECT_EQ( a.is_negative, expected.is_negative );
    EXPECT_EQ( a.type, expected.type );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, OverflowByMantissa ) {
    Error error = get_default_error();
    BigNumber expected = make_inf( error, false );

    digits value( 200019, 1 );
    BigNumber a = make_big_number( value, 0, false, error );

    EXPECT_EQ( a.mantissa, expected.mantissa );
    EXPECT_EQ( a.shift, expected.shift );
    EXPECT_EQ( a.is_negative, expected.is_negative );
    EXPECT_EQ( a.type, expected.type );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, OverflowByExp ) {
    Error error = get_default_error();
    BigNumber expected = make_inf( error, false );

    BigNumber a = make_big_number( { 1 }, MAX_EXP + 1, false, error );

    EXPECT_EQ( a.mantissa, expected.mantissa );
    EXPECT_EQ( a.shift, expected.shift );
    EXPECT_EQ( a.is_negative, expected.is_negative );
    EXPECT_EQ( a.type, expected.type );
    EXPECT_TRUE( is_ok( a.error ) );
}
