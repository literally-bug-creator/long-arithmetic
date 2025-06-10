#include <gtest/gtest.h>
#include <vector>

#include "big_number.hpp"

using namespace big_number;

class ConstructorTest : public ::testing::Test {};

TEST_F( ConstructorTest, MakeBigNumber_ValidChunks ) {
    std::vector<chunk> chunks = { 1, 2, 3 };
    BigNumber result = make_big_number( chunks, 5, false );

    std::vector<chunk> expected_chunks = { 1, 2, 3 };
    EXPECT_EQ( result.chunks, expected_chunks );
    EXPECT_EQ( result.exponent, 5 );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( ConstructorTest, MakeBigNumber_WithLeadingZeros ) {
    std::vector<chunk> chunks = { 0, 0, 5, 6 };
    Error test_error;
    BigNumber result = make_big_number( chunks, 2, true, test_error );

    std::vector<chunk> expected_chunks = { 5, 6 };
    EXPECT_EQ( result.chunks, expected_chunks );
    EXPECT_EQ( result.exponent, 2 + ( -2 ) );
    EXPECT_TRUE( result.is_negative );
}

TEST_F( ConstructorTest, MakeBigNumber_WithTrailingZeros ) {
    std::vector<chunk> chunks = { 7, 8, 0, 0 };
    Error test_error;
    BigNumber result = make_big_number( chunks, -1, false, test_error );

    std::vector<chunk> expected_chunks = { 7, 8 };
    EXPECT_EQ( result.chunks, expected_chunks );
    EXPECT_EQ( result.exponent, -1 + 2 );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( ConstructorTest, MakeBigNumber_AllZerosReturnsZero ) {
    std::vector<chunk> chunks = { 0, 0, 0 };
    Error test_error;
    BigNumber result = make_big_number( chunks, 10, true, test_error );

    EXPECT_TRUE( result.chunks.empty() );
    EXPECT_EQ( result.exponent, 0 );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( ConstructorTest, MakeBigNumber_EmptyChunksReturnsZero ) {
    std::vector<chunk> chunks = {};
    Error test_error;
    BigNumber result = make_big_number( chunks, 5, true, test_error );

    EXPECT_TRUE( result.chunks.empty() );
    EXPECT_EQ( result.exponent, 0 );
    EXPECT_FALSE( result.is_negative );
}

TEST_F( ConstructorTest, MakeBigNumber_SingleChunk ) {
    std::vector<chunk> chunks = { 42 };
    Error test_error;
    BigNumber result = make_big_number( chunks, 0, false, test_error );

    std::vector<chunk> expected_chunks = { 42 };
    EXPECT_EQ( result.chunks, expected_chunks );
    EXPECT_EQ( result.exponent, 0 + ( 0 ) );
    EXPECT_FALSE( result.is_negative );
}
