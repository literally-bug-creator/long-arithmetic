// #include <cmath>
// #include <gmpxx.h>
// #include <gtest/gtest.h>
// #include <string>

// #include "big_number.hpp"

// using namespace std;
// using namespace big_number;

// constexpr int TEST_CASES = 10000;
// constexpr double EPS = 1e-6;

// class BigNumberMulTest : public ::testing::Test {};

// TEST( Multiplication, RandomFloatStrings ) {
//     int mul_ok = 0, mul_fail = 0;
//     for ( int i = 0; i < TEST_CASES; ++i ) {
//         std::string a_str = randomFloatString();
//         std::string b_str = randomFloatString();
//         BigNumber a = make_big_number( a_str );
//         BigNumber b = make_big_number( b_str );
//         BigNumber prod = mul( a, b );
//         mpf_class a_gmp( a_str, 1280 );
//         mpf_class b_gmp( b_str, 1280 );
//         mpf_class prod_gmp = a_gmp * b_gmp;
//         mpf_class prod_bn( to_string( prod ), 1280 );
//         if ( abs( prod_bn - prod_gmp ) < EPS ) {
//             ++mul_ok;
//         } else {
//             ++mul_fail;
//             ADD_FAILURE() << "[MUL FAIL] a=" << a_str << ", b=" << b_str
//                           << ", got=" << prod_bn << ", expected=" << prod_gmp;
//             std::cout << a.exponent << " " << b.exponent << std::endl;
//             std::cout << prod.exponent << std::endl;
//         }
//     }
//     ASSERT_EQ( mul_fail, 0 )
//         << "Multiplication: " << mul_ok << "/" << TEST_CASES << " passed, "
//         << mul_fail << " failed.";
// }

// TEST( Multiplication, Manual_1000x1000_Nines ) {
//     std::string nines_1000( 100000, '9' );
//     BigNumber a = make_big_number( nines_1000 );
//     BigNumber b = make_big_number( nines_1000 );
//     BigNumber prod = mul( a, b );
//     mpz_class a_gmp( nines_1000 );
//     mpz_class b_gmp( nines_1000 );
//     mpz_class prod_gmp = a_gmp * b_gmp;
//     ASSERT_EQ( to_string( prod ), prod_gmp.get_str() )
//         << "Manual 1000x1000 nines failed: got=" << to_string( prod )
//         << ", expected=" << prod_gmp.get_str();
// }

// TEST( Multiplication, Manual_BigTimesOne ) {
//     std::string big( 1000, '9' );
//     BigNumber a = make_big_number( big );
//     BigNumber b = make_big_number( "1" );
//     BigNumber prod = mul( a, b );
//     mpz_class a_gmp( big );
//     mpz_class b_gmp( "1" );
//     mpz_class prod_gmp = a_gmp * b_gmp;
//     ASSERT_EQ( to_string( prod ), prod_gmp.get_str() );
// }

// TEST( Multiplication, Manual_DifferentLength ) {
//     BigNumber a = make_big_number( "12345678901234567890" );
//     BigNumber b = make_big_number( "987654321" );
//     BigNumber prod = mul( a, b );
//     mpz_class a_gmp( "12345678901234567890" );
//     mpz_class b_gmp( "987654321" );
//     mpz_class prod_gmp = a_gmp * b_gmp;
//     ASSERT_EQ( to_string( prod ), prod_gmp.get_str() );
// }

// TEST( Multiplication, Manual_PositiveNegative ) {
//     BigNumber a = make_big_number( "1000000000000000000" );
//     BigNumber b = make_big_number( "-999999999999999999" );
//     BigNumber prod = mul( a, b );
//     mpz_class a_gmp( "1000000000000000000" );
//     mpz_class b_gmp( "-999999999999999999" );
//     mpz_class prod_gmp = a_gmp * b_gmp;
//     ASSERT_EQ( to_string( prod ), prod_gmp.get_str() );
// }

// TEST( Multiplication, Manual_Zero ) {
//     BigNumber a = make_big_number( "0" );
//     BigNumber b = make_big_number( "123456789" );
//     BigNumber prod = mul( a, b );
//     ASSERT_EQ( to_string( prod ), "0" );
// }

// TEST( Multiplication, Manual_BothNegative ) {
//     BigNumber a = make_big_number( "-123456789" );
//     BigNumber b = make_big_number( "-987654321" );
//     BigNumber prod = mul( a, b );
//     mpz_class a_gmp( "-123456789" );
//     mpz_class b_gmp( "-987654321" );
//     mpz_class prod_gmp = a_gmp * b_gmp;
//     ASSERT_EQ( to_string( prod ), prod_gmp.get_str() );
// }

// TEST( Multiplication, Manual_FloatEdge ) {
//     BigNumber a = make_big_number( "0.999999999999999999" );
//     BigNumber b = make_big_number( "0.000000000000000001" );
//     BigNumber prod = mul( a, b );
//     mpf_class a_gmp( "0.999999999999999999", 128 );
//     mpf_class b_gmp( "0.000000000000000001", 128 );
//     mpf_class prod_gmp = a_gmp * b_gmp;
//     mpf_class prod_bn( to_string( prod ), 128 );
//     ASSERT_NEAR( prod_bn.get_d(), prod_gmp.get_d(), 1e-15 );
// }

// TEST( Multiplication, Manual_DifferentExponent ) {
//     BigNumber a = make_big_number( "1e20" );
//     BigNumber b = make_big_number( "1e-10" );
//     BigNumber prod = mul( a, b );
//     mpf_class a_gmp( "1e20", 128 );
//     mpf_class b_gmp( "1e-10", 128 );
//     mpf_class prod_gmp = a_gmp * b_gmp;
//     mpf_class prod_bn( to_string( prod ), 128 );
//     ASSERT_NEAR( prod_bn.get_d(), prod_gmp.get_d(), 1e5 );
// }

// TEST( Multiplication, Manual_BigNegativeTimesBigPositive ) {
//     std::string big_pos( 500, '9' );
//     std::string big_neg = "-" + std::string( 500, '9' );
//     BigNumber a = make_big_number( big_neg );
//     BigNumber b = make_big_number( big_pos );
//     BigNumber prod = mul( a, b );
//     mpz_class a_gmp( big_neg );
//     mpz_class b_gmp( big_pos );
//     mpz_class prod_gmp = a_gmp * b_gmp;
//     ASSERT_EQ( to_string( prod ), prod_gmp.get_str() );
// }

// TEST( Multiplication, Manual_OneTimesMinusOne ) {
//     BigNumber a = make_big_number( "1" );
//     BigNumber b = make_big_number( "-1" );
//     BigNumber prod = mul( a, b );
//     ASSERT_EQ( to_string( prod ), "-1" );
// }

// TEST( Multiplication, Manual_BigFloatTimesBigInt ) {
//     BigNumber a = make_big_number( "999999999999999.999999999999999" );
//     BigNumber b = make_big_number( "1" );
//     BigNumber prod = mul( a, b );
//     mpf_class a_gmp( "999999999999999.999999999999999", 128 );
//     mpf_class b_gmp( "1", 128 );
//     mpf_class prod_gmp = a_gmp * b_gmp;
//     mpf_class prod_bn( to_string( prod ), 128 );
//     ASSERT_NEAR( prod_bn.get_d(), prod_gmp.get_d(), 1e-6 );
// }

// TEST( Multiplication, Manual_NegativeFloatTimesPositiveInt ) {
//     BigNumber a = make_big_number( "-12345.6789" );
//     BigNumber b = make_big_number( "12345" );
//     BigNumber prod = mul( a, b );
//     mpf_class a_gmp( "-12345.6789", 128 );
//     mpf_class b_gmp( "12345", 128 );
//     mpf_class prod_gmp = a_gmp * b_gmp;
//     mpf_class prod_bn( to_string( prod ), 128 );
//     ASSERT_NEAR( prod_bn.get_d(), prod_gmp.get_d(), 1e-6 );
// }

// TEST( Multiplication, Manual_ExponentNotationWithGMP ) {
//     BigNumber a = make_big_number( "2e5" );
//     BigNumber b = make_big_number( "3e-3" );
//     BigNumber prod = mul( a, b );

//     mpf_class a_gmp( 200000, 128 ); // 2e5
//     mpf_class b_gmp( 0.003, 128 );  // 3e-3
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), 128 );
//     ASSERT_NEAR( prod_bn.get_d(), prod_gmp.get_d(), 1e-10 );
// }

// TEST( Multiplication, Manual_VerySmallNumbersWithGMP ) {
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

// TEST( Multiplication, Manual_ExponentBoundaryWithGMP ) {
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

// TEST( Multiplication, Manual_CloseToOneWithGMP ) {
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

// TEST( Multiplication, Manual_FractionalPrecisionLoss ) {
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

// TEST( Multiplication, Manual_MixedSignsWithGMP ) {
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

// TEST( Multiplication, Manual_LargeExponentDifference ) {
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

// TEST( Multiplication, Manual_VeryLargeIntegers ) {
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
// TEST( Multiplication, Manual_LargePositiveExponents ) {
//     BigNumber a = make_big_number( "1.2345e1000" );
//     BigNumber b = make_big_number( "5.6789e2000" );
//     BigNumber prod = mul( a, b );

//     // Устанавливаем высокую точность для GMP
//     constexpr int precision = 8000;
//     mpf_class a_gmp( "1.2345e1000", precision );
//     mpf_class b_gmp( "5.6789e2000", precision );
//     mpf_class prod_gmp = a_gmp * b_gmp;

//     mpf_class prod_bn( to_string( prod ), precision );

//     // Используем сравнение с относительной погрешностью для чисел с плавающей
//     // запятой
//     mpf_class rel_err = abs( prod_bn - prod_gmp ) / abs( prod_gmp );
//     ASSERT_LE( rel_err.get_d(),
//                1e-15 ); // Проверяем, что относительная ошибка очень мала
// }

// // Тест на умножение гигантского числа на очень маленькое (почти ноль).
// // Проверяет обработку экспонент при большой разнице в порядках чисел.
// TEST( Multiplication, Manual_HugeByTiny ) {
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

// TEST( Multiplication, Manual_SquareLargeNegative ) {
//     std::string large_neg_str = "-" + std::string( 2000, '8' );

//     BigNumber a = make_big_number( large_neg_str );
//     BigNumber b = make_big_number( large_neg_str ); // a * a
//     BigNumber prod = mul( a, b );

//     mpz_class a_gmp( large_neg_str );
//     mpz_class prod_gmp = a_gmp * a_gmp;

//     ASSERT_EQ( to_string( prod ), prod_gmp.get_str() );
// }

// TEST( Multiplication, Manual_LargeMantissaBothSides ) {
//     std::string a_str = std::string( 500, '1' ) + "." + std::string( 500, '2' );
//     std::string b_str = std::string( 500, '3' ) + "." + std::string( 500, '4' );

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
