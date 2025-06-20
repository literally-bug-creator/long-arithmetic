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

    EXPECT_EQ( result, "inf" );
}

TEST_F( BigNumberToStringTest, ReturnsNegativeInfinityString ) {
    auto number = big_number::make_inf( big_number::Error{}, true );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "-inf" );
}

TEST_F( BigNumberToStringTest, ReturnsZeroString ) {
    auto number = big_number::make_zero( big_number::Error{} );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "0.0e+0" );
}

TEST_F( BigNumberToStringTest, FormatsThreeDigitPositiveNumber ) {
    chunks chunks = { 123 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "1.23e+2" );
}

TEST_F( BigNumberToStringTest, FormatsThreeDigitNegativeNumber ) {
    chunks chunks = { 456 };
    auto number = createBigNumber( chunks, 0, true );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "-4.56e+2" );
}

TEST_F( BigNumberToStringTest, FormatsSingleDigitPositiveNumber ) {
    chunks chunks = { 7 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "7.e+0" );
}

TEST_F( BigNumberToStringTest, FormatsSingleDigitNegativeNumber ) {
    chunks chunks = { 3 };
    auto number = createBigNumber( chunks, 0, true );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "-3.e+0" );
}

TEST_F( BigNumberToStringTest, FormatsMultipleChunksCorrectly ) {
    chunks chunks = { 789, 456, 123 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result[0] == '1' );
    EXPECT_TRUE( result[1] == '.' );
    EXPECT_TRUE( result.find( "e+" ) != std::string::npos );
}

TEST_F( BigNumberToStringTest, HandlesPositiveShiftCorrectly ) {
    chunks chunks = { 123 };
    auto number = createBigNumber( chunks, 2, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result[0] == '1' );
    EXPECT_TRUE( result[1] == '.' );
    EXPECT_TRUE( result.find( "e+" ) != std::string::npos );
}

TEST_F( BigNumberToStringTest, HandlesNegativeShiftCorrectly ) {
    chunks chunks = { 123 };
    auto number = createBigNumber( chunks, -1, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result[0] == '1' );
    EXPECT_TRUE( result[1] == '.' );
    std::string exponent_part = result.substr( result.find( 'e' ) );
    EXPECT_TRUE( exponent_part.find( "e-" ) != std::string::npos ||
                 exponent_part.find( "e+" ) != std::string::npos );
}

TEST_F( BigNumberToStringTest, RemovesTrailingZerosFromFraction ) {
    chunks chunks = { 1200 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "1.2e+3" );
}

TEST_F( BigNumberToStringTest, FormatsNumberWithAllZerosAfterFirstDigit ) {
    chunks chunks = { 1000 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "1.0e+3" );
}

TEST_F( BigNumberToStringTest, HandlesLargePositiveShift ) {
    chunks chunks = { 123 };
    auto number = createBigNumber( chunks, 1000, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result.find( "e+" ) != std::string::npos );
    std::string exponent_str = result.substr( result.find( "e+" ) + 2 );
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
    EXPECT_GT( exponent, 1000 );
}

TEST_F( BigNumberToStringTest, HandlesMaximumChunkValue ) {
    chunks chunks = { 999999999 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result[0] == '9' );
    EXPECT_TRUE( result[1] == '.' );
    EXPECT_TRUE( result.find( "e+" ) != std::string::npos );
}

TEST_F( BigNumberToStringTest, HandlesMinimumChunkValue ) {
    chunks chunks = { 1 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "1.e+0" );
}

TEST_F( BigNumberToStringTest, FormatsComplexPositiveMultiChunkNumber ) {
    chunks chunks = { 987654321, 123456789, 555 };
    auto number = createBigNumber( chunks, 5, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result[0] == '5' );
    EXPECT_TRUE( result[1] == '.' );
    EXPECT_TRUE( result.find( "e+" ) != std::string::npos );
    EXPECT_FALSE( result.empty() );
}

TEST_F( BigNumberToStringTest, FormatsComplexNegativeMultiChunkNumber ) {
    chunks chunks = { 111222333, 444555666, 777 };
    auto number = createBigNumber( chunks, -3, true );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result[0] == '-' );
    EXPECT_TRUE( result[1] == '7' );
    EXPECT_TRUE( result[2] == '.' );
    EXPECT_TRUE( result.find( "e" ) != std::string::npos );
}

TEST_F( BigNumberToStringTest, FormatsNumberWithZeroExponent ) {
    chunks chunks = { 1 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result.find( "e+0" ) != std::string::npos );
}

TEST_F( BigNumberToStringTest, RemovesAllTrailingZerosFromSingleDigit ) {
    chunks chunks = { 5 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "5.e+0" );
}

TEST_F( BigNumberToStringTest, RemovesMultipleTrailingZeros ) {
    chunks chunks = { 1230000 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "1.23e+6" );
}

TEST_F( BigNumberToStringTest, RemovesTrailingZerosButKeepsSignificantDigits ) {
    chunks chunks = { 1500 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_EQ( result, "1.5e+3" );
}

TEST_F( BigNumberToStringTest, FormatsSmallPositiveExponent ) {
    chunks chunks = { 12 };
    auto number = createBigNumber( chunks, 0, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result.find( "e+1" ) != std::string::npos );
}

TEST_F( BigNumberToStringTest, FormatsSmallNegativeExponentOrPositive ) {
    chunks chunks = { 1 };
    auto number = createBigNumber( chunks, -1, false );

    std::string result = big_number::to_string( number );

    EXPECT_TRUE( result.find( "e-" ) != std::string::npos ||
                 result.find( "e+" ) != std::string::npos );
}
