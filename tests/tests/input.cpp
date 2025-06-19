#include <cstddef>
#include <cstdint>
#include <gtest/gtest.h>

#include "big_number.hpp"
#include "error.hpp"

using namespace big_number;

class InputTest : public ::testing::Test {};

TEST_F( InputTest, ZeroDigitBasicCase ) {
    chunks expected_chunks = {};
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 0 }, 0, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, EmptyDigitsArrayZeroExponent ) {
    chunks expected_chunks = {};
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number( {}, 0, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, ThreeDigitsZeroExponent ) {
    chunks expected_chunks = { 123 };
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 1, 2, 3 }, 0, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, NegativeSignBasicCase ) {
    chunks expected_chunks = { 123 };
    int32_t expected_exp = 0;
    bool expected_sign = true;

    BigNumber a = make_big_number( { 1, 2, 3 }, 0, true, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, PositiveExponent1 ) {
    chunks expected_chunks = { 1230 };
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 1, 2, 3 }, 1, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, SingleDigitWithNegativeExponent ) {
    chunks expected_chunks = { 3 };
    int32_t expected_exp = -1;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 3 }, -18, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, ExactlyOneChunk18Digits ) {
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

TEST_F( InputTest, MaxValueSingleChunk18Nines ) {
    chunks expected_chunks = { 999999999999999999 };
    int32_t expected_exp = 0;
    bool expected_sign = false;
    digits digits = { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 };

    BigNumber a = make_big_number( digits, 0, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, TwoChunks21Digits ) {
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

TEST_F( InputTest, ExactlyTwoChunks36Digits ) {
    chunks expected_chunks = { 123456789012345678, 456789012345678901 };
    int32_t expected_exp = 0;
    bool expected_sign = false;
    digits digits = { 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1,
                      1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 };

    BigNumber a = make_big_number( digits, 0, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, LeadingZero ) {
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

TEST_F( InputTest, TrailingZeros ) {
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

TEST_F( InputTest, ZerosInMiddleOfDigitsArray ) {
    chunks expected_chunks = { 1020300000 };
    int32_t expected_exp = -1;
    bool expected_sign = false;

    BigNumber a =
        make_big_number( { 1, 0, 2, 0, 3 }, -13, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, PositiveExponentDivisibleByBase ) {
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

TEST_F( InputTest, NegativeExponentDivisibleByBase ) {
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

TEST_F( InputTest, NegativeExponentNotDivisibleByBase ) {
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

TEST_F( InputTest, NegativeExponentRemainderOne ) {
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

TEST_F( InputTest, TwoDigitsWithNegativeExponent ) {
    chunks expected_chunks = { 1200 };
    int32_t expected_exp = -1;
    bool expected_sign = false;

    BigNumber a = make_big_number( { 1, 2 }, -16, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, NegativeExponentRemainder17 ) {
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

TEST_F( InputTest, PositiveExponentRemainder17 ) {
    chunks expected_chunks = { 300000000000000000, 12 };
    int32_t expected_exp = 0;
    bool expected_sign = false;

    BigNumber a =
        make_big_number( { 1, 2, 3 }, 17, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, NegativeExponentRemainder19 ) {
    chunks expected_chunks = { 300000000000000000, 12 };
    int32_t expected_exp = -2;
    bool expected_sign = false;

    BigNumber a =
        make_big_number( { 1, 2, 3 }, -19, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, PositiveExponentRemainder19 ) {
    chunks expected_chunks = { 1230 };
    int32_t expected_exp = 1;
    bool expected_sign = false;

    BigNumber a =
        make_big_number( { 1, 2, 3 }, 19, false, get_default_error() );

    EXPECT_EQ( a.mantissa, expected_chunks );
    EXPECT_EQ( a.shift, expected_exp );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, MaxDigitsWithMaxExponent ) {
    Error error = get_default_error();
    size_t expected_mantissa_size = MAX_CHUNKS;
    int32_t expected_shift = MAX_SHIFT;
    bool expected_sign = false;
    BigNumberType expected_type = BigNumberType::DEFAULT;
    digits digits( MAX_DIGITS, 1 );

    BigNumber a = make_big_number( digits, MAX_EXP, false, error );

    EXPECT_EQ( a.mantissa.size(), expected_mantissa_size );
    EXPECT_EQ( a.shift, expected_shift );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_EQ( a.type, expected_type );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, MaxDigitsWithNegativeMaxExponent ) {
    Error error = get_default_error();
    size_t expected_mantissa_size = MAX_CHUNKS;
    int32_t expected_shift = -MAX_SHIFT;
    bool expected_sign = false;
    BigNumberType expected_type = BigNumberType::DEFAULT;
    digits digits( MAX_DIGITS, 1 );

    BigNumber a = make_big_number( digits, -MAX_EXP, false, error );

    EXPECT_EQ( a.mantissa.size(), expected_mantissa_size );
    EXPECT_EQ( a.shift, expected_shift );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_EQ( a.type, expected_type );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, DigitsTruncationToMaxDigits ) {
    Error error = get_default_error();
    size_t oversized_digits = MAX_DIGITS + 1;
    digits large_digits( oversized_digits, 9 );

    BigNumber a = make_big_number( large_digits, 0, false, error );

    EXPECT_LE( a.mantissa.size(), MAX_CHUNKS );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, TruncationWithExponentCompensation ) {
    Error error = get_default_error();
    BigNumberType expected_type = BigNumberType::DEFAULT;
    bool expected_sign = false;
    size_t oversized_digits = MAX_DIGITS + BASE;
    digits large_digits( oversized_digits, 1 );

    BigNumber a = make_big_number( large_digits, 0, false, error );

    EXPECT_EQ( a.mantissa.size(), MAX_CHUNKS );
    EXPECT_EQ( a.type, expected_type );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, DigitTruncationWithNegativeExponent ) {
    Error error = get_default_error();
    size_t oversized = MAX_DIGITS + 100;
    digits large_digits( oversized, 1 );

    BigNumber a = make_big_number( large_digits, -MAX_EXP, false, error );

    EXPECT_LE( a.mantissa.size(), MAX_CHUNKS );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, OverflowToPositiveInfinity ) {
    Error error = get_default_error();
    BigNumber expected = make_inf( error, false );
    int32_t delta = BASE;
    digits digits( MAX_DIGITS + delta, 1 );

    BigNumber a = make_big_number( digits, MAX_EXP, false, error );

    EXPECT_EQ( a.mantissa, expected.mantissa );
    EXPECT_EQ( a.shift, expected.shift );
    EXPECT_EQ( a.is_negative, expected.is_negative );
    EXPECT_EQ( a.type, expected.type );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, OverflowToNegativeInfinity ) {
    Error error = get_default_error();
    BigNumber expected = make_inf( error, true );
    int32_t delta = BASE;
    digits digits( MAX_DIGITS + delta, 1 );

    BigNumber a = make_big_number( digits, MAX_EXP, true, error );

    EXPECT_EQ( a.mantissa, expected.mantissa );
    EXPECT_EQ( a.shift, expected.shift );
    EXPECT_EQ( a.is_negative, expected.is_negative );
    EXPECT_EQ( a.type, expected.type );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, OverflowDigitsWithNegativeMaxExponent ) {
    Error error = get_default_error();
    size_t expected_mantissa_size = MAX_CHUNKS;
    int32_t delta = BASE;
    int32_t expected_shift = -MAX_SHIFT + delta / BASE;
    bool expected_sign = false;
    bool expected_type = BigNumberType::DEFAULT;
    digits digits( MAX_DIGITS + delta, 1 );

    BigNumber a = make_big_number( digits, -MAX_EXP, false, error );

    EXPECT_EQ( a.mantissa.size(), expected_mantissa_size );
    EXPECT_EQ( a.shift, expected_shift );
    EXPECT_EQ( a.is_negative, expected_sign );
    EXPECT_EQ( a.type, expected_type );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, ExponentOverflowToInfinity ) {
    Error error = get_default_error();
    BigNumber expected_inf = make_inf( error, false );

    BigNumber a = make_big_number( { 1 }, MAX_DIGITS + BASE, false, error );

    EXPECT_EQ( a.type, expected_inf.type );
    EXPECT_EQ( a.is_negative, expected_inf.is_negative );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, ExponentCompensationOverflowToInfinity ) {
    Error error = get_default_error();
    size_t delta_size = MAX_EXP + 1;
    digits oversized( MAX_DIGITS + delta_size, 1 );

    BigNumber a = make_big_number( oversized, 0, false, error );

    EXPECT_TRUE( a.type == BigNumberType::INF );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, UnderflowToZero ) {
    Error error = get_default_error();
    BigNumber expected = make_zero( error );
    int32_t delta = BASE;
    digits digits( MAX_DIGITS, 1 );

    BigNumber a = make_big_number( digits, -MAX_EXP - delta, false, error );

    EXPECT_EQ( a.mantissa, expected.mantissa );
    EXPECT_EQ( a.shift, expected.shift );
    EXPECT_EQ( a.is_negative, expected.is_negative );
    EXPECT_EQ( a.type, expected.type );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, ExponentUnderflowToZero ) {
    Error error = get_default_error();
    BigNumber expected_zero = make_zero( error );

    BigNumber a = make_big_number( { 1 }, -MAX_EXP - BASE, false, error );

    EXPECT_EQ( a.type, expected_zero.type );
    EXPECT_EQ( a.mantissa, expected_zero.mantissa );
    EXPECT_EQ( a.shift, expected_zero.shift );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, VeryLargeDigitsWithMaxExponent ) {
    Error error = get_default_error();
    size_t oversized_digits = MAX_DIGITS * 3;
    digits large_digits( oversized_digits, 9 );

    BigNumber a = make_big_number( large_digits, MAX_EXP, false, error );

    EXPECT_TRUE( a.type == BigNumberType::INF ||
                 a.mantissa.size() <= MAX_CHUNKS );
    EXPECT_TRUE( is_ok( a.error ) );
}

TEST_F( InputTest, ErrorCodePropagation ) {
    Error input_error = make_error( ErrorCode::ERROR );

    BigNumber a = make_big_number( { 1, 2, 3 }, 0, false, input_error );

    EXPECT_FALSE( is_ok( a.error ) );
    EXPECT_EQ( a.error.code, ErrorCode::ERROR );
}
