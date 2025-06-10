#include <gtest/gtest.h>
#include <vector>

#include "big_number.hpp"

using namespace big_number;

class ConstructorTest : public ::testing::Test {};

TEST_F( ConstructorTest, MakeBigNumber_ValidChunks ) {
    std::vector<chunk> chunks = { 1, 2, 3 };
    BigNumber expected_result = make_big_number( chunks, 5, false );

    BigNumber result = make_big_number( chunks, 5, false );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( ConstructorTest, MakeBigNumber_WithLeadingZeros ) {
    std::vector<chunk> chunks = { 0, 0, 5, 6 };
    BigNumber expected_result = make_big_number( { 5, 6 }, 2, true );

    BigNumber result = make_big_number( chunks, 2, true );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( ConstructorTest, MakeBigNumber_WithTrailingZeros ) {
    std::vector<chunk> chunks = { 7, 8, 0, 0 };
    BigNumber expected_result = make_big_number( { 7, 8 }, -1, false );

    BigNumber result = make_big_number( chunks, -1, false );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( ConstructorTest, MakeBigNumber_AllZerosReturnsZero ) {
    std::vector<chunk> chunks = { 0, 0, 0 };
    BigNumber expected_result = make_big_number( {}, 0, false );

    BigNumber result = make_big_number( chunks, 10, true );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( ConstructorTest, MakeBigNumber_EmptyChunksReturnsZero ) {
    std::vector<chunk> chunks = {};
    BigNumber expected_result = make_big_number( {}, 0, false );

    BigNumber result = make_big_number( chunks, 5, true );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}

TEST_F( ConstructorTest, MakeBigNumber_SingleChunk ) {
    std::vector<chunk> chunks = { 42 };
    BigNumber expected_result = make_big_number( chunks, 0, false );

    BigNumber result = make_big_number( chunks, 0, false );

    EXPECT_TRUE( is_equal( result, expected_result ) );
}
