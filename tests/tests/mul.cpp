#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "tools.hpp"

using namespace big_number;

class BigNumberMulTest : public ::testing::Test {
protected:
    Error error = get_default_error();
};

TEST_F( BigNumberMulTest, MultiplyByZero ) {
    BigNumber zero = make_zero( error );
    BigNumber number = create_big_number( { 123 }, 0, false );
    BigNumber expected = make_zero( error );

    BigNumber result = mul( zero, number );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyZeroByNumber ) {
    BigNumber number = create_big_number( { 123 }, 0, false );
    BigNumber zero = make_zero( error );
    BigNumber expected = make_zero( error );

    BigNumber result = mul( number, zero );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyTwoZeros ) {
    BigNumber zero1 = make_zero( error );
    BigNumber zero2 = make_zero( error );
    BigNumber expected = make_zero( error );

    BigNumber result = mul( zero1, zero2 );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyByOne ) {
    BigNumber one = create_big_number( { 1 }, 0, false );
    BigNumber number = create_big_number( { 123 }, 0, false );

    BigNumber result = mul( one, number );

    for ( auto& chunk : result.mantissa ) {
        std::cout << chunk << "\t";
    }

    std::cout << std::endl;

    EXPECT_TRUE( is_equal( result, number ) );
}

TEST_F( BigNumberMulTest, MultiplyOneByNumber ) {
    BigNumber number = create_big_number( { 123 }, 0, false );
    BigNumber one = create_big_number( { 1 }, 0, false );

    BigNumber result = mul( number, one );

    EXPECT_TRUE( is_equal( result, number ) );
}

TEST_F( BigNumberMulTest, MultiplySmallNumbers ) {
    BigNumber a = create_big_number( { 2 }, 0, false );
    BigNumber b = create_big_number( { 3 }, 0, false );
    BigNumber expected = create_big_number( { 6 }, 0, false );

    BigNumber result = mul( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyTwoPositiveNumbers ) {
    BigNumber a = create_big_number( { 123 }, 0, false );
    BigNumber b = create_big_number( { 456 }, 0, false );
    BigNumber expected = create_big_number( { 56088 }, 0, false );

    BigNumber result = mul( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyTwoNegativeNumbers ) {
    BigNumber a = create_big_number( { 123 }, 0, true );
    BigNumber b = create_big_number( { 456 }, 0, true );
    BigNumber expected = create_big_number( { 56088 }, 0, false );

    BigNumber result = mul( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyPositiveAndNegative ) {
    BigNumber a = create_big_number( { 123 }, 0, false );
    BigNumber b = create_big_number( { 456 }, 0, true );
    BigNumber expected = create_big_number( { 56088 }, 0, true );

    BigNumber result = mul( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyNegativeAndPositive ) {
    BigNumber a = create_big_number( { 123 }, 0, true );
    BigNumber b = create_big_number( { 456 }, 0, false );
    BigNumber expected = create_big_number( { 56088 }, 0, true );

    BigNumber result = mul( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyWithDifferentShifts ) {
    BigNumber a = create_big_number( { 123 }, 0, false );
    BigNumber b = create_big_number( { 456 }, 2, false );
    BigNumber expected = create_big_number( { 56088 }, 2, false );

    BigNumber result = mul( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyWithCarry ) {
    BigNumber a = create_big_number( { 999999999999999999ULL }, 0, false );
    BigNumber b = create_big_number( { 2 }, 0, false );
    BigNumber expected =
        create_big_number( { 999999999999999998ULL, 1 }, 0, false );

    BigNumber result = mul( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyMultipleChunks ) {
    BigNumber a = create_big_number( { 123, 456 }, 0, false );
    BigNumber b = create_big_number( { 789 }, 0, false );
    BigNumber expected = create_big_number( { 97047, 359784 }, 0, false );

    BigNumber result = mul( a, b );

    EXPECT_TRUE( is_equal( result, expected ) );
}

TEST_F( BigNumberMulTest, MultiplyLargeNumbersSimple ) {
    chunks a_chunks( 50, 123456789 );
    chunks b_chunks( 50, 987654321 );
    BigNumber a = create_big_number( a_chunks, 0, false );
    BigNumber b = create_big_number( b_chunks, 0, false );

    BigNumber result = mul( a, b );

    EXPECT_FALSE( is_equal( result, make_zero( error ) ) );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( BigNumberMulTest, MultiplyVeryLargeNumbersNTT ) {
    chunks a_chunks( 200, 123456789 );
    chunks b_chunks( 200, 987654321 );
    BigNumber a = create_big_number( a_chunks, 0, false );
    BigNumber b = create_big_number( b_chunks, 0, false );

    BigNumber result = mul( a, b );

    EXPECT_FALSE( is_equal( result, make_zero( error ) ) );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( BigNumberMulTest, MultiplyNanByNumber ) {
    BigNumber nan = make_nan( error );
    BigNumber number = create_big_number( { 123 }, 0, false );

    BigNumber result = mul( nan, number );

    EXPECT_TRUE( result.type == BigNumberType::NOT_A_NUMBER );
}

TEST_F( BigNumberMulTest, MultiplyNumberByNan ) {
    BigNumber number = create_big_number( { 123 }, 0, false );
    BigNumber nan = make_nan( error );

    BigNumber result = mul( number, nan );

    EXPECT_TRUE( result.type == BigNumberType::NOT_A_NUMBER );
}

TEST_F( BigNumberMulTest, MultiplyPositiveInfinityByNumber ) {
    BigNumber inf = make_inf( error, false );
    BigNumber number = create_big_number( { 123 }, 0, false );

    BigNumber result = mul( inf, number );

    EXPECT_TRUE( result.type == BigNumberType::INF );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( BigNumberMulTest, MultiplyNegativeInfinityByPositiveNumber ) {
    BigNumber inf = make_inf( error, true );
    BigNumber number = create_big_number( { 123 }, 0, false );

    BigNumber result = mul( inf, number );

    EXPECT_TRUE( result.type == BigNumberType::INF );
    EXPECT_TRUE( result.is_negative );
}

TEST_F( BigNumberMulTest, MultiplyPositiveInfinityByNegativeNumber ) {
    BigNumber inf = make_inf( error, false );
    BigNumber number = create_big_number( { 123 }, 0, true );

    BigNumber result = mul( inf, number );

    EXPECT_TRUE( result.type == BigNumberType::INF );
    EXPECT_TRUE( result.is_negative );
}

TEST_F( BigNumberMulTest, MultiplyTwoPositiveInfinities ) {
    BigNumber inf1 = make_inf( error, false );
    BigNumber inf2 = make_inf( error, false );

    BigNumber result = mul( inf1, inf2 );

    EXPECT_TRUE( result.type == BigNumberType::INF );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( BigNumberMulTest, MultiplyTwoNegativeInfinities ) {
    BigNumber inf1 = make_inf( error, true );
    BigNumber inf2 = make_inf( error, true );

    BigNumber result = mul( inf1, inf2 );

    EXPECT_TRUE( result.type == BigNumberType::INF );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( BigNumberMulTest, MultiplyPositiveAndNegativeInfinity ) {
    BigNumber pos_inf = make_inf( error, false );
    BigNumber neg_inf = make_inf( error, true );

    BigNumber result = mul( pos_inf, neg_inf );

    EXPECT_TRUE( result.type == BigNumberType::INF );
    EXPECT_TRUE( result.is_negative );
}

TEST_F( BigNumberMulTest, MultiplyInfinityByZero ) {
    BigNumber inf = make_inf( error, false );
    BigNumber zero = make_zero( error );

    BigNumber result = mul( inf, zero );

    EXPECT_TRUE( result.type == BigNumberType::NOT_A_NUMBER );
}

TEST_F( BigNumberMulTest, MultiplyZeroByInfinity ) {
    BigNumber zero = make_zero( error );
    BigNumber inf = make_inf( error, false );

    BigNumber result = mul( zero, inf );

    EXPECT_TRUE( result.type == BigNumberType::NOT_A_NUMBER );
}
