// #include <gtest/gtest.h>

// #include "big_number.hpp"
// using namespace big_number;

// class BigNumberMulTest : public ::testing::Test {};

// TEST_F( BigNumberMulTest, ZeroMultiplication ) {
//     BigNumber a = make_big_number( { 1, 2, 3 }, 0 );
//     BigNumber b = make_big_number( { 0 }, 0 );
//     BigNumber expected = make_big_number( { 0 }, 0 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, MultiplyByOne ) {
//     BigNumber a = make_big_number( { 4, 2 }, 0 );
//     BigNumber b = make_big_number( { 1 }, 0 );
//     BigNumber expected = make_big_number( { 4, 2 }, 0 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, SimplePositiveNumbers ) {
//     BigNumber a = make_big_number( { 1, 2 }, 0 );
//     BigNumber b = make_big_number( { 3, 4 }, 0 );
//     BigNumber expected = make_big_number( { 4, 0, 8 }, 0 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, PositiveByNegative ) {
//     BigNumber a = make_big_number( { 5, 0 }, 0 );
//     BigNumber b = make_big_number( { 6 }, 0, true );
//     BigNumber expected = make_big_number( { 3, 0, 0 }, 0, true );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, NegativeByNegative ) {
//     BigNumber a = make_big_number( { 7 }, 0, true );
//     BigNumber b = make_big_number( { 8 }, 0, true );
//     BigNumber expected = make_big_number( { 5, 6 }, 0 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, SingleDigitMultiplication ) {
//     BigNumber a = make_big_number( { 3 }, 0 );
//     BigNumber b = make_big_number( { 4 }, 0 );
//     BigNumber expected = make_big_number( { 1, 2 }, 0 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, MultipleDigits ) {
//     BigNumber a = make_big_number( { 1, 2, 3 }, 0 );
//     BigNumber b = make_big_number( { 4, 5, 6 }, 0 );
//     BigNumber expected = make_big_number( { 5, 6, 0, 8, 8 }, 0 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, WithPositiveExponents ) {
//     BigNumber a = make_big_number( { 1, 2 }, 1 );
//     BigNumber b = make_big_number( { 3 }, 1 );
//     BigNumber expected = make_big_number( { 3, 6 }, 2 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, WithNegativeExponents ) {
//     BigNumber a = make_big_number( { 2 }, -1 );
//     BigNumber b = make_big_number( { 5 }, -1 );
//     BigNumber expected = make_big_number( { 1 }, -1 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, MixedExponents ) {
//     BigNumber a = make_big_number( { 2 }, 1 );
//     BigNumber b = make_big_number( { 5 }, -1 );
//     BigNumber expected = make_big_number( { 1 }, 1 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, PowersOfTen ) {
//     BigNumber a = make_big_number( { 1 }, 2 );
//     BigNumber b = make_big_number( { 1 }, 3 );
//     BigNumber expected = make_big_number( { 1 }, 5 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, LargeNumber ) {
//     BigNumber a = make_big_number( { 9, 9, 9 }, 0 );
//     BigNumber b = make_big_number( { 9, 9, 9 }, 0 );
//     BigNumber expected = make_big_number( { 9, 9, 8, 0, 0, 1 }, 0 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberMulTest, NumberWithTrailingZeros ) {
//     BigNumber a = make_big_number( { 1, 0, 0 }, 0 );
//     BigNumber b = make_big_number( { 2, 0, 0 }, 0 );
//     BigNumber expected = make_big_number( { 2, 0, 0, 0, 0 }, 0 );

//     BigNumber result = mul( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }
