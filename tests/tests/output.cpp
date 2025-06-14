#include <gtest/gtest.h>

#include "big_number.hpp"

using namespace big_number;

class ToStringTest : public ::testing::Test {};

typedef std::vector<chunk> chunks;
typedef std::vector<int> digits;

TEST_F( ToStringTest, EmptyChunks ) {
    std::string expected = "0";

    BigNumber num = make_big_number();

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, PositiveIntegerZeroExponent ) {
    std::string expected = "123";

    BigNumber num = make_big_number( { 1, 2, 3 } );

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, NegativeIntegerZeroExponent ) {
    std::string expected = "-456";

    BigNumber num = make_big_number( { 4, 5, 6 }, 0, true );

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, PositiveNumberNegativeExponent ) {
    std::string expected = "789e-18";

    BigNumber num = make_big_number( { 7, 8, 9 }, -18, false );

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, NegativeNumberNegativeExponent ) {
    std::string expected = "-321e-1";

    BigNumber num = make_big_number( { 3, 2, 1 }, -1, true );

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, PositiveNumberPositiveExponent ) {
    std::string expected = "654e2";

    BigNumber num = make_big_number( { 6, 5, 4 }, 2 );

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, NegativeNumberPositiveExponent ) {
    std::string expected = "-987e3";

    BigNumber num = make_big_number( { 9, 8, 7 }, 3, true );

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, MultipleChunksZeroExponent ) {
    std::string expected = "123456";

    BigNumber num = make_big_number( { 1, 2, 3, 4, 5, 6 } );

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, MultipleChunksPositiveExponent ) {
    std::string expected = "12345678e1";

    BigNumber num = make_big_number( { 1, 2, 3, 4, 5, 6, 7, 8 }, 1 );

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, MultipleChunksNegativeExponent ) {
    std::string expected = "-222111e-2";

    BigNumber num = make_big_number( { 2, 2, 2, 1, 1, 1 }, -2, true );

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, LargePositiveExponent ) {
    std::string expected = "1e1800";

    BigNumber num = make_big_number( { 1 }, 1800 );

    EXPECT_EQ( to_string( num ), expected );
}

TEST_F( ToStringTest, LargeNegativeExponent ) {
    std::string expected = "-1e-100";

    BigNumber num = make_big_number( { 1 }, -100, true );

    EXPECT_EQ( to_string( num ), expected );
}
