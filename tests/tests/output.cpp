#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "error.hpp"

using namespace big_number;

class BigNumberToStringTest : public ::testing::Test {
protected:
    big_number::BigNumber createBigNumber(
        const chunks& chunks,
        int32_t shift,
        bool is_negative = false,
        big_number::BigNumberType type = big_number::BigNumberType::DEFAULT ) {
        big_number::BigNumber number;
        number.mantissa = chunks;
        number.shift = shift;
        number.is_negative = is_negative;
        number.type = type;
        number.error = big_number::Error{};
        return number;
    }
};

TEST_F( BigNumberToStringTest, ReturnsNaNString ) {
    auto number = big_number::make_nan( big_number::Error{} );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "NaN" );
}

TEST_F( BigNumberToStringTest, ReturnsPositiveInfinityString ) {
    auto number = big_number::make_inf( big_number::Error{}, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "INF" );
}

TEST_F( BigNumberToStringTest, ReturnsNegativeInfinityString ) {
    auto number = big_number::make_inf( big_number::Error{}, true );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "-INF" );
}

TEST_F( BigNumberToStringTest, ReturnsZeroString ) {
    auto number = big_number::make_zero( big_number::Error{} );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "0" );
}

TEST_F( BigNumberToStringTest, FormatsSingleDigitPositiveNumber ) {
    chunks chunks = { 7 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "7" );
}

TEST_F( BigNumberToStringTest, FormatsSingleDigitNegativeNumber ) {
    chunks chunks = { 3 };
    auto number = createBigNumber( chunks, 0, true );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "-3" );
}

TEST_F( BigNumberToStringTest, FormatsMultiDigitPositiveNumber ) {
    chunks chunks = { 123 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "123" );
}

TEST_F( BigNumberToStringTest, FormatsMultiDigitNegativeNumber ) {
    chunks chunks = { 456 };
    auto number = createBigNumber( chunks, 0, true );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "-456" );
}

TEST_F( BigNumberToStringTest, HandlesTrailingZerosInNumber ) {
    chunks chunks = { 1200 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "1200" );
}

TEST_F( BigNumberToStringTest, HandlesNumberWithManyTrailingZeros ) {
    chunks chunks = { 1230000 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "1230000" );
}

TEST_F( BigNumberToStringTest, HandlesMinimumChunkValue ) {
    chunks chunks = { 1 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "1" );
}

TEST_F( BigNumberToStringTest, HandlesMaximumChunkValue ) {
    chunks chunks = { 999999999 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result.find( "999999999" ) != std::string::npos );
    EXPECT_TRUE( result.find( "e" ) == std::string::npos );
}

TEST_F( BigNumberToStringTest, HandlesSmallPositiveShift ) {
    chunks chunks = { 123 };
    auto number = createBigNumber( chunks, 2, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result.find( "123" ) != std::string::npos );
    EXPECT_TRUE( result.find( "e" ) != std::string::npos );
    std::string exp_part = result.substr( result.find( "e" ) + 2 );
    int exp = std::stoi( exp_part );
    EXPECT_GT( exp, 0 );
}

TEST_F( BigNumberToStringTest, HandlesSmallNegativeShift ) {
    chunks chunks = { 123 };
    auto number = createBigNumber( chunks, -1, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result.find( "123" ) != std::string::npos );
    EXPECT_TRUE( result.find( "e-" ) != std::string::npos );
}

TEST_F( BigNumberToStringTest, FormatsMultipleChunksNumber ) {
    chunks chunks = { 789, 456, 123 };
    auto number = createBigNumber( chunks, 0, false );
    std::string expected = "123" + std::string( BASE - 3, '0' ) + "456" +
                           std::string( BASE - 3, '0' ) + "789";

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, expected );
}

TEST_F( BigNumberToStringTest, HandlesLargePositiveShift ) {
    chunks chunks = { 123 };
    auto number = createBigNumber( chunks, 1000, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result.find( "e" ) != std::string::npos );
    std::string exponent_str = result.substr( result.find( "e" ) + 2 );
    int exponent = std::stoi( exponent_str );
    EXPECT_GT( exponent, 1000 );
}

TEST_F( BigNumberToStringTest, HandlesLargeNegativeShift ) {
    chunks chunks = { 123 };
    auto number = createBigNumber( chunks, -1000, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result.find( "e-" ) != std::string::npos );
    std::string exponent_str = result.substr( result.find( "e-" ) + 2 );
    int exponent = std::stoi( exponent_str );
    EXPECT_EQ( exponent, -1000 * BASE );
}

TEST_F( BigNumberToStringTest,
        FormatsComplexPositiveMultiChunkNumberWithShift ) {
    chunks chunks = { 987654321, 123456789, 555 };
    auto number = createBigNumber( chunks, 5, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result.find( "555" ) != std::string::npos );
    EXPECT_TRUE( result.find( "e" ) != std::string::npos );
    EXPECT_FALSE( result.empty() );
}

TEST_F( BigNumberToStringTest,
        FormatsComplexNegativeMultiChunkNumberWithShift ) {
    chunks chunks = { 111222333, 444555666, 777 };
    auto number = createBigNumber( chunks, -3, true );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result[0] == '-' );
    EXPECT_TRUE( result.find( "777" ) != std::string::npos );
    EXPECT_TRUE( result.find( "e" ) != std::string::npos );
}
