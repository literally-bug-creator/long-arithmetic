#include <cstdint>
#include <gmpxx.h>
#include <gtest/gtest.h>
#include <random>
#include <string>

#include "big_number.hpp"
#include "iostream"

using namespace big_number;

const u_long SEED = 42;
const int32_t TEST_F_CASES = 10000;
const double EPS = 1e-6;

class BigNumberMulTest : public ::testing::Test {
public:
    std::vector<chunk> random_chunks( int min_chunks, int max_chunks ) {
        std::vector<chunk> chunks;

        std::uniform_int_distribution<int> chunk_count_dist( min_chunks,
                                                             max_chunks );
        int chunk_count = chunk_count_dist( rng );

        std::uniform_int_distribution<chunk> chunk_dist( 1, 1000000 );
        chunks.reserve( chunk_count );

        for ( int i = 0; i < chunk_count; ++i ) {
            chunks.push_back( chunk_dist( rng ) );
        }

        if ( !chunks.empty() && chunks[0] == 0 ) { chunks[0] = 1; }

        return chunks;
    }

    int32_t random_exp( int32_t min_exponent, int32_t max_exponent ) {
        std::uniform_int_distribution<int32_t> exp_dist( min_exponent,
                                                         max_exponent );
        return exp_dist( rng );
    }

    bool random_sign() {
        std::uniform_int_distribution<int> sign_dist( 0, 1 );
        return static_cast<bool>( sign_dist( rng ) );
    }

    BigNumber random_big_number( int min_chunks = 1,
                                 int max_chunks = 5,
                                 int32_t min_exponent = -5,
                                 int32_t max_exponent = 5 ) {
        std::vector<chunk> chunks = random_chunks( min_chunks, max_chunks );
        int32_t exponent = random_exp( min_exponent, max_exponent );
        bool is_negative = random_sign();

        return make_big_number( chunks, exponent, is_negative );
    }

    BigNumber from_chunks( std::vector<chunk> chunks ) {
        return make_big_number( chunks, 0, false );
    }

    std::pair<std::string, int32_t> parse_parts( const std::string& str ) {
        size_t ePos = str.find_first_of( "eE" );

        if ( ePos == std::string::npos ) { return { str, 0 }; }

        std::string mantissa = str.substr( 0, ePos );
        int exponent = std::stoi( str.substr( ePos + 1 ) );

        return { mantissa, exponent };
    }

    mpz_class long_int_from_big_number( const BigNumber& number ) {
        auto mantissa_exp = parse_parts( to_string( number ) );
        std::string str =
            mantissa_exp.first + std::string( mantissa_exp.second, '0' );

        return mpz_class( str );
    }

    mpf_class long_float_from_big_number( const BigNumber& number ) {
        return mpf_class( to_string( number ), 1280 );
    }

    bool is_equal( const BigNumber& number, const mpz_class right ) {
        mpz_class left = long_int_from_big_number( number );
        return left.get_str() == right.get_str();
    }

    bool is_equal( const BigNumber& number, const mpf_class right ) {
        mpf_class left = long_float_from_big_number( number );
        return ( left - right ) < EPS;
    }

protected:
    void SetUp() override { rng.seed( SEED ); }

    std::mt19937 rng;
};

TEST_F( BigNumberMulTest, Random ) {
    for ( int i = 0; i < TEST_F_CASES; i++ ) {
        BigNumber a = random_big_number();
        BigNumber b = random_big_number();
        mpf_class expected =
            long_float_from_big_number( a ) * long_float_from_big_number( b );

        BigNumber real = mul( a, b );

        EXPECT_TRUE( is_equal( real, expected ) );
    }
}

TEST_F( BigNumberMulTest, Manual1000x1000_Nines ) {
    int32_t length = 100000 / CHUNK_DIGITS;
    std::vector<chunk> chunks( length, 999999999999999999 );
    BigNumber a = from_chunks( chunks );
    BigNumber b = from_chunks( chunks );
    mpz_class expected =
        long_int_from_big_number( a ) * long_int_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_BigTimesOne ) {
    int32_t length = 1000 / CHUNK_DIGITS;
    std::vector<chunk> chunks( length, 999999999999999999 );
    BigNumber a = from_chunks( chunks );
    BigNumber b = make_big_number( { 1 }, 0, false );
    mpz_class expected =
        long_int_from_big_number( a ) * long_int_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_DifferentLength ) {
    BigNumber a = make_big_number( { 1234567890, 1234567890 }, 0, false );
    BigNumber b = make_big_number( { 987654321 }, 0, false );
    mpz_class expected =
        long_int_from_big_number( a ) * long_int_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_PositiveNegative ) {
    BigNumber a = make_big_number( { 1 }, 1, false );
    BigNumber b = make_big_number( { 999999999999999999 }, 0, true );
    mpz_class expected =
        long_int_from_big_number( a ) * long_int_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_Zero ) {
    BigNumber a = make_big_number( {}, 0, false );
    BigNumber b = make_big_number( { 123456789 }, 0, false );
    mpz_class expected =
        long_int_from_big_number( a ) * long_int_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_BothNegative ) {
    BigNumber a = make_big_number( { 123456789 }, 0, true );
    BigNumber b = make_big_number( { 987654321 }, 0, true );
    mpz_class expected =
        long_int_from_big_number( a ) * long_int_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_FloatEdge ) {
    BigNumber a = make_big_number( { 999999999999999999 }, -1, false );
    BigNumber b = make_big_number( { 1 }, -1, false );
    mpf_class expected =
        long_float_from_big_number( a ) * long_float_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_DifferentExponent ) {
    BigNumber a = make_big_number( { 100 }, 20, false );
    BigNumber b = make_big_number( { 100000000 }, -1, false );
    mpf_class expected =
        long_float_from_big_number( a ) * long_float_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_BigNegativeTimesBigPositive ) {
    int32_t length = 500 / CHUNK_DIGITS;
    chunk base_chunk = 999999999999999999;
    BigNumber a = from_chunks( std::vector<chunk>( length, base_chunk ) );
    BigNumber b =
        make_big_number( std::vector<chunk>( length, base_chunk ), 0, true );
    mpz_class expected =
        long_int_from_big_number( a ) * long_int_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_OneTimesMinusOne ) {
    BigNumber a = make_big_number( { 1 }, 0, false );
    BigNumber b = make_big_number( { 1 }, 0, true );
    BigNumber expected = make_big_number( { 1 }, 0, true );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( big_number::is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_BigFloatTimesBigInt ) {
    BigNumber a =
        make_big_number( { 99999999999999999, 9999999999999 }, -1, false );
    BigNumber b = make_big_number( { 1 }, 0, false );
    mpf_class expected =
        long_float_from_big_number( a ) * long_float_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

TEST_F( BigNumberMulTest, Manual_NegativeFloatTimesPositiveInt ) {
    BigNumber a = make_big_number( { 12345, 678900000000000000 }, -1, true );
    BigNumber b = make_big_number( { 12345 }, 0, false );
    mpf_class expected =
        long_float_from_big_number( a ) * long_float_from_big_number( b );

    BigNumber real = mul( a, b );

    EXPECT_TRUE( is_equal( real, expected ) );
}

// TEST_F( BigNumberMulTest, Manual_ExponentNotationWithGMP ) {
//     BigNumber a = make_big_number( { 200000 }, 0, false );
//     BigNumber b = make_big_number( "0.003" );
//     mpf_class expected =
//         long_float_from_big_number( a ) * long_float_from_big_number( b );

//     BigNumber real = mul( a, b );

//     EXPECT_TRUE( is_equal( real, expected ) );
// }

// TEST_F( BigNumberMulTEST_F, Manual_VerySmallNumbersWithGMP ) {
//     BigNumber a = make_big_number( "0.0000000000000000001" );
//     BigNumber b = make_big_number( "0.0000000000000000001" );
//     BigNumber prod = mul( a, b );

//     mpf_class a_gmp( "0.0000000000000000001", 256 );
//     mpf_class b_gmp( "0.0000000000000000001", 256 );
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), 256 );
//     if ( prod_gmp != 0 ) {
//         mpf_class rel_err = abs( prod_bn - prod_gmp ) / abs( prod_gmp );
//         ASSERT_LE( rel_err.get_d(), 1e-10 );
//     } else {
//         ASSERT_EQ( prod_bn, prod_gmp );
//     }
// }

// TEST_F( BigNumberMulTEST_F, Manual_ExponentBoundaryWithGMP ) {
//     BigNumber a = make_big_number( "1e500" );
//     BigNumber b = make_big_number( "1e-500" );
//     BigNumber prod = mul( a, b );

//     mpf_class a_gmp( 0, 2000 );
//     mpf_class b_gmp( 0, 2000 );
//     a_gmp = "1e500";
//     b_gmp = "1e-500";
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), 2000 );
//     ASSERT_NEAR( prod_bn.get_d(), prod_gmp.get_d(), 1e-9 );
// }

// TEST_F( BigNumberMulTEST_F, Manual_CloseToOneWithGMP ) {
//     BigNumber a = make_big_number( "0.99999999999999999999" );
//     BigNumber b = make_big_number( "0.99999999999999999999" );
//     BigNumber prod = mul( a, b );

//     mpf_class a_gmp( "0.99999999999999999999", 512 );
//     mpf_class b_gmp( "0.99999999999999999999", 512 );
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), 512 );
//     mpf_class abs_err = abs( prod_bn - prod_gmp );
//     ASSERT_LE( abs_err.get_d(), 1e-30 );
// }

// TEST_F( BigNumberMulTEST_F, Manual_FractionalPrecisionLoss ) {
//     BigNumber a = make_big_number( "0.1234567890123456789" );
//     BigNumber b = make_big_number( "0.9876543210987654321" );
//     BigNumber prod = mul( a, b );

//     mpf_class a_gmp( "0.1234567890123456789", 256 );
//     mpf_class b_gmp( "0.9876543210987654321", 256 );
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), 256 );
//     if ( prod_gmp != 0 ) {
//         mpf_class rel_err = abs( prod_bn - prod_gmp ) / abs( prod_gmp );
//         ASSERT_LE( rel_err.get_d(), 1e-15 );
//     } else {
//         ASSERT_EQ( prod_bn, prod_gmp );
//     }
// }

// TEST_F( BigNumberMulTEST_F, Manual_MixedSignsWithGMP ) {
//     BigNumber a = make_big_number( "-12345678901234567890.123456789" );
//     BigNumber b = make_big_number( "9876543210987654321.987654321" );
//     BigNumber prod = mul( a, b );

//     mpf_class a_gmp( "-12345678901234567890.123456789", 256 );
//     mpf_class b_gmp( "9876543210987654321.987654321", 256 );
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), 256 );
//     if ( prod_gmp != 0 ) {
//         mpf_class rel_err = abs( prod_bn - prod_gmp ) / abs( prod_gmp );
//         ASSERT_LE( rel_err.get_d(), 1e-15 );
//     } else {
//         ASSERT_EQ( prod_bn, prod_gmp );
//     }
// }

// TEST_F( BigNumberMulTEST_F, Manual_LargeExponentDifference ) {
//     BigNumber a = make_big_number( "1e300" );
//     BigNumber b = make_big_number( "1e-300" );
//     BigNumber prod = mul( a, b );

//     mpf_class a_gmp( "1e300", 2000 );
//     mpf_class b_gmp( "1e-300", 2000 );
//     // a_gmp = "1e300";
//     // b_gmp = "1e-300";
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), 2000 );
//     ASSERT_NEAR( prod_bn.get_d(), prod_gmp.get_d(), 1e-9 );
// }

// TEST_F( BigNumberMulTEST_F, Manual_VeryLargeIntegers ) {
//     // Создаем два числа: одно с 10000 знаков, другое с 5000.
//     std::string big_a_str( 10000, '1' );
//     big_a_str[0] = '5'; // Делаем их немного разными
//     std::string big_b_str( 5000, '2' );
//     big_b_str[0] = '7';

//     BigNumber a = make_big_number( big_a_str );
//     BigNumber b = make_big_number( big_b_str );
//     BigNumber prod = mul( a, b );

//     mpz_class a_gmp( big_a_str );
//     mpz_class b_gmp( big_b_str );
//     mpz_class prod_gmp = a_gmp * b_gmp;

//     ASSERT_EQ( to_string( prod ), prod_gmp.get_str() );
// }

// // Тест на перемножение двух чисел с большими положительными экспонентами.
// // Проверяет корректность обработки и сложения экспонент.
// TEST_F( BigNumberMulTEST_F, Manual_LargePositiveExponents ) {
//     BigNumber a = make_big_number( "1.2345e1000" );
//     BigNumber b = make_big_number( "5.6789e2000" );
//     BigNumber prod = mul( a, b );

//     // Устанавливаем высокую точность для GMP
//     constexpr int precision = 8000;
//     mpf_class a_gmp( "1.2345e1000", precision );
//     mpf_class b_gmp( "5.6789e2000", precision );
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), precision );

//     // Используем сравнение с относительной погрешностью для чисел с
//     плавающей
//     // запятой
//     mpf_class rel_err = abs( prod_bn - prod_gmp ) / abs( prod_gmp );
//     ASSERT_LE( rel_err.get_d(),
//                1e-15 ); // Проверяем, что относительная ошибка очень мала
// }

// // Тест на умножение гигантского числа на очень маленькое (почти ноль).
// // Проверяет обработку экспонент при большой разнице в порядках чисел.
// TEST_F( BigNumberMulTEST_F, Manual_HugeByTiny ) {
//     BigNumber a = make_big_number( "9.999999e5000" );
//     BigNumber b = make_big_number( "1.0e-4999" );
//     BigNumber prod = mul( a, b );

//     constexpr int precision = 20000;
//     mpf_class a_gmp( "9.999999e5000", precision );
//     mpf_class b_gmp( "1.0e-4999", precision );
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), precision );

//     mpf_class abs_err = abs( prod_bn - prod_gmp );
//     ASSERT_LE( abs_err.get_d(), 1e-10 );
// }

// TEST_F( BigNumberMulTEST_F, Manual_SquareLargeNegative ) {
//     std::string large_neg_str = "-" + std::string( 2000, '8' );

//     BigNumber a = make_big_number( large_neg_str );
//     BigNumber b = make_big_number( large_neg_str ); // a * a
//     BigNumber prod = mul( a, b );

//     mpz_class a_gmp( large_neg_str );
//     mpz_class prod_gmp = a_gmp * a_gmp;

//     ASSERT_EQ( to_string( prod ), prod_gmp.get_str() );
// }

// TEST_F( BigNumberMulTEST_F, Manual_LargeMantissaBothSides ) {
//     std::string a_str = std::string( 500, '1' ) + "." + std::string( 500, '2'
//     ); std::string b_str = std::string( 500, '3' ) + "." + std::string( 500,
//     '4' );

//     BigNumber a = make_big_number( a_str );
//     BigNumber b = make_big_number( b_str );
//     BigNumber prod = mul( a, b );

//     constexpr int precision = 4000;
//     mpf_class a_gmp( a_str, precision );
//     mpf_class b_gmp( b_str, precision );
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), precision );

//     mpf_class rel_err = abs( prod_bn - prod_gmp ) / abs( prod_gmp );
//     ASSERT_LE( rel_err.get_d(), 1e-15 );
// }
