// #include <gtest/gtest.h>

// #include "big_number.hpp"

// using namespace big_number;

// class BigNumberAbsNegTest : public ::testing::Test {};

// TEST_F( BigNumberAbsNegTest, AbsPositiveNumber ) {
//     BigNumber positive = make_big_number( { 123, 456 }, 5, false );

//     BigNumber result = abs( positive );

//     EXPECT_TRUE( is_equal( result, positive ) );
//     EXPECT_TRUE( is_ok( get_error( result ) ) );
// }

// TEST_F( BigNumberAbsNegTest, AbsNegativeNumber ) {
//     BigNumber negative = make_big_number( { 789, 12 }, 3, true );
//     BigNumber expected = make_big_number( { 789, 12 }, 3, false );

//     BigNumber result = abs( negative );

//     EXPECT_TRUE( is_equal( result, expected ) );
//     EXPECT_TRUE( is_ok( get_error( result ) ) );
// }

// TEST_F( BigNumberAbsNegTest, AbsZero ) {
//     BigNumber zero = make_big_number( { 0 }, 0, true );
//     BigNumber expected_zero = make_big_number( { 0 }, 0, false );

//     BigNumber result = abs( zero );

//     EXPECT_TRUE( is_equal( result, expected_zero ) );
//     EXPECT_TRUE( is_ok( get_error( result ) ) );
// }

// TEST_F( BigNumberAbsNegTest, NegPositiveNumber ) {
//     BigNumber positive = make_big_number( { 555, 666 }, 7, false );
//     BigNumber expected = make_big_number( { 555, 666 }, 7, true );

//     BigNumber result = neg( positive );

//     EXPECT_TRUE( is_equal( result, expected ) );
//     EXPECT_TRUE( is_ok( get_error( result ) ) );
// }

// TEST_F( BigNumberAbsNegTest, NegNegativeNumber ) {
//     BigNumber negative = make_big_number( { 777, 888 }, 4, true );
//     BigNumber expected = make_big_number( { 777, 888 }, 4, false );

//     BigNumber result = neg( negative );

//     EXPECT_TRUE( is_equal( result, expected ) );
//     EXPECT_TRUE( is_ok( get_error( result ) ) );
// }

// TEST_F( BigNumberAbsNegTest, DoubleNegation ) {
//     BigNumber original = make_big_number( { 123, 456 }, 2, false );

//     BigNumber doubleNeg = neg( neg( original ) );

//     EXPECT_TRUE( is_equal( doubleNeg, original ) );
//     EXPECT_TRUE( is_ok( get_error( doubleNeg ) ) );
// }

// TEST_F( BigNumberAbsNegTest, AbsOfNegation ) {
//     BigNumber original = make_big_number( { 987, 654 }, 3, false );

//     BigNumber absNeg = abs( neg( original ) );
//     BigNumber absOrig = abs( original );

//     EXPECT_TRUE( is_equal( absNeg, absOrig ) );
//     EXPECT_TRUE( is_ok( get_error( absNeg ) ) );
//     EXPECT_TRUE( is_ok( get_error( absOrig ) ) );
// }
