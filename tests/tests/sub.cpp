// #include <gtest/gtest.h>

// #include "big_number.hpp"

// using namespace big_number;

// class BigNumberSubTest : public ::testing::Test {};

// TEST_F( BigNumberSubTest, SamePositiveNumbers ) {
//     BigNumber a = make_big_number( { 1, 2, 3 }, 0 );
//     BigNumber b = make_big_number( { 1, 2, 3 }, 0 );
//     BigNumber expected = make_big_number( { 0 }, 0 );

//     BigNumber result = sub( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberSubTest, PositiveMinusSmaller ) {
//     BigNumber a = make_big_number( { 1, 0, 0 }, 0 );
//     BigNumber b = make_big_number( { 2, 5 }, 0 );
//     BigNumber expected = make_big_number( { 7, 5 }, 0 );

//     BigNumber result = sub( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberSubTest, PositiveMinusLarger ) {
//     BigNumber a = make_big_number( { 2, 5 }, 0 );
//     BigNumber b = make_big_number( { 1, 0, 0 }, 0 );
//     BigNumber expected = make_big_number( { 7, 5 }, 0, true );

//     BigNumber result = sub( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberSubTest, PositiveMinusNegative ) {
//     BigNumber a = make_big_number( { 5, 0 }, 0 );
//     BigNumber b = make_big_number( { 3, 0 }, 0, true );
//     BigNumber expected = make_big_number( { 8, 0 }, 0 );

//     BigNumber result = sub( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberSubTest, NegativeMinusPositive ) {
//     BigNumber a = make_big_number( { 4, 0 }, 0, true );
//     BigNumber b = make_big_number( { 2, 0 }, 0 );
//     BigNumber expected = make_big_number( { 6, 0 }, 0, true );

//     BigNumber result = sub( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberSubTest, NegativeMinusNegative ) {
//     BigNumber a = make_big_number( { 3, 0 }, 0, true );
//     BigNumber b = make_big_number( { 1, 0 }, 0, true );
//     BigNumber expected = make_big_number( { 2, 0 }, 0, true );

//     BigNumber result = sub( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberSubTest, SubtractFromZero ) {
//     BigNumber a = make_big_number( { 0 }, 0 );
//     BigNumber b = make_big_number( { 4, 2 }, 0 );
//     BigNumber expected = make_big_number( { 4, 2 }, 0, true );

//     BigNumber result = sub( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberSubTest, SubtractZero ) {
//     BigNumber a = make_big_number( { 1, 2, 3 }, 0 );
//     BigNumber b = make_big_number( { 0 }, 0 );
//     BigNumber expected = make_big_number( { 1, 2, 3 }, 0 );

//     BigNumber result = sub( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberSubTest, LargeNumbers ) {
//     BigNumber a = make_big_number(
//         { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 }, 0 );
//     BigNumber b = make_big_number( { 1 }, 0 );
//     BigNumber expected = make_big_number(
//         { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 8 }, 0 );

//     BigNumber result = sub( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }

// TEST_F( BigNumberSubTest, WithExponents ) {
//     BigNumber a = make_big_number( { 1, 5 }, 3 );
//     BigNumber b = make_big_number( { 5 }, 2 );
//     BigNumber expected = make_big_number( { 1, 4, 5 }, 2 );

//     BigNumber result = sub( a, b );

//     EXPECT_TRUE( is_equal( result, expected ) );
// }
