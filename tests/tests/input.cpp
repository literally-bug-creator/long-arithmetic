#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "tools.hpp"

using namespace big_number;

class InputTest : public ::testing::Test {
protected:
    Error error = get_default_error();
};

TEST_F( InputTest, ZeroDigitBasicCase ) {
    BigNumber expected = make_zero( error );

    BigNumber result = make_big_number( { 0 }, 0, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, EmptyDigitsArrayZeroExponent ) {
    BigNumber expected = make_zero( error );

    BigNumber result = make_big_number( {}, 0, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, ThreeDigitsZeroExponent ) {
    BigNumber expected = create_big_number( { 123 }, 0, false );

    BigNumber result = make_big_number( { 1, 2, 3 }, 0, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, NegativeSignBasicCase ) {
    BigNumber expected = create_big_number( { 123 }, 0, true );

    BigNumber result = make_big_number( { 1, 2, 3 }, 0, true, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, PositiveExponent1 ) {
    BigNumber expected = create_big_number( { 1230 }, 0, false );

    BigNumber result = make_big_number( { 1, 2, 3 }, 1, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, SingleDigitWithNegativeExponent ) {
    BigNumber expected = create_big_number( { 3 }, -1, false );

    BigNumber result = make_big_number( { 3 }, -18, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, ExactlyOneChunk18Digits ) {
    BigNumber expected = create_big_number( { 123456789012345678 }, 0, false );

    BigNumber result = make_big_number(
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 },
        0,
        false,
        error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, MaxValueSingleChunk18Nines ) {
    BigNumber expected = create_big_number( { 999999999999999999 }, 0, false );

    BigNumber result = make_big_number(
        { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 },
        0,
        false,
        error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, TwoChunks21Digits ) {
    BigNumber expected =
        create_big_number( { 123456789012345678, 456 }, 0, false );

    BigNumber result = make_big_number(
        { 4, 5, 6, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 },
        0,
        false,
        error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, ExactlyTwoChunks36Digits ) {
    BigNumber expected = create_big_number(
        { 123456789012345678, 456789012345678901 }, 0, false );

    BigNumber result = make_big_number( { 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5,
                                          6, 7, 8, 9, 0, 1, 1, 2, 3, 4, 5, 6,
                                          7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 },
                                        0,
                                        false,
                                        error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, LeadingZero ) {
    BigNumber expected = make_zero( error );

    BigNumber result = make_big_number( { 0, 0, 1, 2, 3 }, 0, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, RemoveTrailingZeros ) {
    chunks expected_chunks = { 123456 };
    BigNumber expected = create_big_number( expected_chunks, 1, false );

    BigNumber result = make_big_number( { 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                        0,
                                        false,
                                        error );

    for ( chunk a : result.mantissa ) {
        std::cout << a << std::endl;
    }

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, TrailingZeros ) {
    BigNumber expected = create_big_number( { 123456000 }, 0, false );

    BigNumber result =
        make_big_number( { 1, 2, 3, 4, 5, 6, 0, 0, 0 }, 0, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, ZerosInMiddleOfDigitsArray ) {
    BigNumber expected = create_big_number( { 1020300000 }, -1, false );

    BigNumber result = make_big_number( { 1, 0, 2, 0, 3 }, -13, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, PositiveExponentDivisibleByBase ) {
    BigNumber expected = create_big_number( { 123 }, 2, false );

    BigNumber result = make_big_number( { 1, 2, 3 }, 36, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, NegativeExponentDivisibleByBase ) {
    BigNumber expected = create_big_number( { 123 }, -2, false );

    BigNumber result = make_big_number( { 1, 2, 3 }, -36, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, NegativeExponentNotDivisibleByBase ) {
    BigNumber expected = create_big_number( { 1230000000000000 }, -1, false );

    BigNumber result = make_big_number( { 1, 2, 3 }, -5, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, NegativeExponentRemainderOne ) {
    BigNumber expected =
        create_big_number( { 300000000000000000, 12 }, -1, false );

    BigNumber result = make_big_number( { 1, 2, 3 }, -1, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, TwoDigitsWithNegativeExponent ) {
    BigNumber expected = create_big_number( { 1200 }, -1, false );

    BigNumber result = make_big_number( { 1, 2 }, -16, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, NegativeExponentRemainder17 ) {
    BigNumber expected = create_big_number( { 1230 }, -1, false );

    BigNumber result = make_big_number( { 1, 2, 3 }, -17, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, PositiveExponentRemainder17 ) {
    BigNumber expected =
        create_big_number( { 300000000000000000, 12 }, 0, false );

    BigNumber result = make_big_number( { 1, 2, 3 }, 17, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, NegativeExponentRemainder19 ) {
    BigNumber expected =
        create_big_number( { 300000000000000000, 12 }, -2, false );

    BigNumber result = make_big_number( { 1, 2, 3 }, -19, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, PositiveExponentRemainder19 ) {
    BigNumber expected = create_big_number( { 1230 }, 1, false );

    BigNumber result = make_big_number( { 1, 2, 3 }, 19, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, MaxDigitsWithMaxExponent ) {
    chunks mantissa( MAX_CHUNKS, 111111111111111111 );
    BigNumber expected = create_big_number( mantissa, MAX_SHIFT, false );
    digits digits( MAX_DIGITS, 1 );

    BigNumber result = make_big_number( digits, MAX_EXP, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, MaxDigitsWithNegativeMaxExponent ) {
    chunks mantissa( MAX_CHUNKS, 111111111111111111 );
    BigNumber expected = create_big_number( mantissa, -MAX_SHIFT, false );
    digits digits( MAX_DIGITS, 1 );

    BigNumber result = make_big_number( digits, -MAX_EXP, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, DigitsTruncationToMaxDigits ) {
    BigNumber expected = make_inf( error, false );
    digits digits( MAX_DIGITS + 1, 1 );

    BigNumber result = make_big_number( digits, 0, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, TruncationWithExponentCompensation ) {
    BigNumber expected = make_inf( error, false );
    digits digits( MAX_DIGITS + BASE, 1 );

    BigNumber result = make_big_number( digits, 0, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, DigitTruncationWithNegativeExponent ) {
    BigNumber expected = make_inf( error, false );
    digits digits( MAX_DIGITS + 100, 1 );

    BigNumber result = make_big_number( digits, -MAX_EXP, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, OverflowToPositiveInfinity ) {
    BigNumber expected = make_inf( error, false );
    digits digits( MAX_DIGITS + BASE, 1 );

    BigNumber result = make_big_number( digits, MAX_EXP, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, OverflowToNegativeInfinity ) {
    BigNumber expected = make_inf( error, true );
    digits digits( MAX_DIGITS + BASE, 1 );

    BigNumber result = make_big_number( digits, MAX_EXP, true, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, ExponentOverflowToInfinity ) {
    BigNumber expected = make_inf( error, false );

    BigNumber result =
        make_big_number( { 1 }, MAX_DIGITS + BASE, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, UnderflowToZero ) {
    BigNumber expected = make_zero( error );
    digits digits( MAX_DIGITS, 1 );

    BigNumber result = make_big_number( digits, -MAX_EXP - BASE, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, ExponentUnderflowToZero ) {
    BigNumber expected = make_zero( error );

    BigNumber result = make_big_number( { 1 }, -MAX_EXP - BASE, false, error );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( InputTest, ErrorCodePropagation ) {
    Error input_error = make_error( ErrorCode::ERROR );

    BigNumber result = make_big_number( { 1, 2, 3 }, 0, false, input_error );
    Error result_error = get_error( result );

    EXPECT_FALSE( is_ok( result_error ) );
    EXPECT_EQ( get_error_code( result_error ), ErrorCode::ERROR );
}
