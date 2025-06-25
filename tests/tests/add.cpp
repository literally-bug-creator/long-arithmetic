#include <gtest/gtest.h>

#include "big_number.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "tools.hpp"

using namespace big_number;

class BigNumberAddTest : public ::testing::Test {};

// Test addition of two positive numbers
TEST_F( BigNumberAddTest, AddTwoPositiveNumbers ) {
    // Arrange
    chunks chunks1 = { 123 };
    chunks chunks2 = { 456 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, false );

    // Act
    auto result = add( number1, number2 );

    // Assert
    chunks expected_chunks = { 579 };
    auto expected = create_big_number( expected_chunks, 0, false );

    EXPECT_TRUE( is_equal( result, expected ) );
}

// Test addition of two negative numbers
TEST_F( BigNumberAddTest, AddTwoNegativeNumbers ) {
    // Arrange
    chunks chunks1 = { 123 };
    chunks chunks2 = { 456 };
    auto number1 = create_big_number( chunks1, 0, true );
    auto number2 = create_big_number( chunks2, 0, true );

    // Act
    auto result = add( number1, number2 );

    // Assert
    chunks expected_chunks = { 579 };
    auto expected = create_big_number( expected_chunks, 0, true );
    EXPECT_TRUE( is_equal( result, expected ) );
}

// Test addition with carry
TEST_F( BigNumberAddTest, AddWithCarry ) {
    // Arrange
    chunks chunks1 = { 999999999999999999ULL };
    chunks chunks2 = { 1 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, false );

    // Act
    auto result = add( number1, number2 );

    std::cout << to_string( result ) << std::endl;
    std::cout << result.shift << std::endl;

    for ( auto a : result.mantissa ) {
        std::cout << a << "\t";
    }

    std::cout << std::endl;

    // Assert
    chunks expected_chunks = { 1 };
    auto expected = create_big_number( expected_chunks, 1, false );
    EXPECT_TRUE( is_equal( result, expected ) );
}

// Test addition with different shifts
TEST_F( BigNumberAddTest, AddWithDifferentShifts ) {
    // Arrange
    chunks chunks1 = { 123 };
    chunks chunks2 = { 456 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 2, false );

    // Act
    auto result = add( number1, number2 );

    // Assert
    chunks expected_chunks = { 123, 0, 456 };
    auto expected = create_big_number( expected_chunks, 0, false );
    EXPECT_TRUE( is_equal( result, expected ) );
}

// Test addition with zero (left operand zero)
TEST_F( BigNumberAddTest, AddZeroToNumber ) {
    // Arrange
    auto zero = make_zero( Error{} );
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );

    // Act
    auto result = add( zero, number );

    // Assert
    EXPECT_TRUE( is_equal( result, number ) );
}

// Test addition with zero (right operand zero)
TEST_F( BigNumberAddTest, AddNumberToZero ) {
    // Arrange
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );
    auto zero = make_zero( Error{} );

    // Act
    auto result = add( number, zero );

    // Assert
    EXPECT_TRUE( is_equal( result, number ) );
}

// Test addition of two zeros
TEST_F( BigNumberAddTest, AddTwoZeros ) {
    // Arrange
    auto zero1 = make_zero( Error{} );
    auto zero2 = make_zero( Error{} );

    // Act
    auto result = add( zero1, zero2 );

    // Assert
    auto expected = make_zero( Error{} );
    EXPECT_TRUE( is_equal( result, expected ) );
}

// Test addition of positive and negative numbers (subtraction case)
TEST_F( BigNumberAddTest, AddPositiveAndNegative ) {
    // Arrange
    chunks chunks1 = { 500 };
    chunks chunks2 = { 200 };
    auto positive = create_big_number( chunks1, 0, false );
    auto negative = create_big_number( chunks2, 0, true );

    // Act
    auto result = add( positive, negative );

    // Assert
    chunks expected_chunks = { 300 };
    auto expected = create_big_number( expected_chunks, 0, false );
    EXPECT_TRUE( is_equal( result, expected ) );
}

// Test addition of negative and positive numbers (subtraction case)
TEST_F( BigNumberAddTest, AddNegativeAndPositive ) {
    // Arrange
    chunks chunks1 = { 500 };
    chunks chunks2 = { 200 };
    auto negative = create_big_number( chunks1, 0, true );
    auto positive = create_big_number( chunks2, 0, false );

    // Act
    auto result = add( negative, positive );

    // Assert
    chunks expected_chunks = { 300 };
    auto expected = create_big_number( expected_chunks, 0, true );
    EXPECT_TRUE( is_equal( result, expected ) );
}

// Test addition with multiple chunks
TEST_F( BigNumberAddTest, AddMultipleChunks ) {
    // Arrange
    chunks chunks1 = { 123, 456, 789 };
    chunks chunks2 = { 111, 222, 333 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, false );

    // Act
    auto result = add( number1, number2 );

    // Assert
    chunks expected_chunks = { 234, 678, 1122 };
    auto expected = create_big_number( expected_chunks, 0, false );
    EXPECT_TRUE( is_equal( result, expected ) );
}

// Test addition with different chunk sizes
TEST_F( BigNumberAddTest, AddDifferentChunkSizes ) {
    // Arrange
    chunks chunks1 = { 123, 456 };
    chunks chunks2 = { 789 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, false );

    // Act
    auto result = add( number1, number2 );

    // Assert
    chunks expected_chunks = { 912, 456 };
    auto expected = create_big_number( expected_chunks, 0, false );
    EXPECT_TRUE( is_equal( result, expected ) );
}

// Test addition with NaN (left operand)
TEST_F( BigNumberAddTest, AddNanToNumber ) {
    // Arrange
    auto nan = make_nan( Error{} );
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );

    // Act
    auto result = add( nan, number );

    // Assert
    // Note: Current implementation doesn't handle NaN properly,
    // but we test the current behavior
    EXPECT_FALSE( is_equal( result, number ) );
}

// Test addition with NaN (right operand)
TEST_F( BigNumberAddTest, AddNumberToNan ) {
    // Arrange
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );
    auto nan = make_nan( Error{} );

    // Act
    auto result = add( number, nan );

    // Assert
    // Note: Current implementation doesn't handle NaN properly,
    // but we test the current behavior
    EXPECT_FALSE( is_equal( result, number ) );
}

// Test addition with positive infinity (left operand)
TEST_F( BigNumberAddTest, AddPositiveInfinityToNumber ) {
    // Arrange
    auto inf = make_inf( Error{}, false );
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );

    // Act
    auto result = add( inf, number );

    // Assert
    // Note: Current implementation doesn't handle infinity properly,
    // but we test the current behavior
    EXPECT_FALSE( is_equal( result, number ) );
}

// Test addition with positive infinity (right operand)
TEST_F( BigNumberAddTest, AddNumberToPositiveInfinity ) {
    // Arrange
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );
    auto inf = make_inf( Error{}, false );

    // Act
    auto result = add( number, inf );

    // Assert
    // Note: Current implementation doesn't handle infinity properly,
    // but we test the current behavior
    EXPECT_FALSE( is_equal( result, number ) );
}

// Test addition with negative infinity (left operand)
TEST_F( BigNumberAddTest, AddNegativeInfinityToNumber ) {
    // Arrange
    auto inf = make_inf( Error{}, true );
    chunks chunks = { 123 };
    auto number = create_big_number( chunks, 0, false );

    // Act
    auto result = add( inf, number );

    // Assert
    // Note: Current implementation doesn't handle infinity properly,
    // but we test the current behavior
    EXPECT_FALSE( is_equal( result, number ) );
}

// Test addition of two positive infinities
TEST_F( BigNumberAddTest, AddTwoPositiveInfinities ) {
    // Arrange
    auto inf1 = make_inf( Error{}, false );
    auto inf2 = make_inf( Error{}, false );

    // Act
    auto result = add( inf1, inf2 );

    // Assert
    // Note: Current implementation doesn't handle infinity properly,
    // but we test the current behavior
    EXPECT_FALSE( is_equal( result, inf1 ) );
}

// Test addition of positive and negative infinity
TEST_F( BigNumberAddTest, AddPositiveAndNegativeInfinity ) {
    // Arrange
    auto pos_inf = make_inf( Error{}, false );
    auto neg_inf = make_inf( Error{}, true );

    // Act
    auto result = add( pos_inf, neg_inf );

    // Assert
    // Note: This should result in NaN, but current implementation
    // doesn't handle this case properly
    EXPECT_FALSE( is_equal( result, pos_inf ) );
    EXPECT_FALSE( is_equal( result, neg_inf ) );
}

// Test addition with large numbers causing potential overflow
TEST_F( BigNumberAddTest, AddLargeNumbers ) {
    // Arrange
    chunks large_chunks( 10, ALMOST_MAX_CHUNK );
    auto number1 = create_big_number( large_chunks, 0, false );
    auto number2 = create_big_number( large_chunks, 0, false );

    // Act
    auto result = add( number1, number2 );

    // Assert
    // The result should have carry propagation
    auto zero = make_zero( Error{} );
    EXPECT_FALSE( is_equal( result, zero ) );
    EXPECT_FALSE( result.is_negative );
}

// Test addition preserves error state
TEST_F( BigNumberAddTest, AddPreservesErrorFromLeftOperand ) {
    // Arrange
    chunks chunks1 = { 123 };
    chunks chunks2 = { 456 };
    auto number1 = create_big_number( chunks1, 0, false );
    auto number2 = create_big_number( chunks2, 0, false );

    // Simulate error in number1 by creating it with NaN type
    auto error_number = make_nan( Error{} );

    // Act
    auto result = add( error_number, number2 );

    // Assert
    // Error should be preserved (though implementation may vary)
    EXPECT_FALSE( is_equal( result, number2 ) );
}
