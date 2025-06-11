// #include <gtest/gtest.h>
// #include "big_number.hpp"
// #include "randomString.h"
// #include <gmpxx.h>
// #include <cmath>
// #include <string>

// using namespace std;
// using namespace big_number;

// constexpr int TEST_CASES = 10000;
// constexpr double EPS = 1e-6;

// TEST(Division, RandomFloatStrings) {
//     int div_ok = 0, div_fail = 0;
//     for (int i = 0; i < TEST_CASES; ++i) {
//         std::string a_str = randomFloatString();
//         std::string b_str = randomFloatString();
//         if (b_str == "0" || b_str == "0.0" || b_str == "-0" || b_str == "-0.0") {
//             --i;
//             continue;
//         }
//         BigNumber a = make_big_number(a_str);
//         BigNumber b = make_big_number(b_str);
//         BigNumber quot = div(a, b);
//         mpf_class a_gmp(a_str, 1280);
//         mpf_class b_gmp(b_str, 1280);
//         mpf_class quot_gmp = a_gmp / b_gmp;
//         mpf_class quot_bn(to_string(quot), 1280);
//         if (abs(quot_bn - quot_gmp) < EPS) {
//             ++div_ok;
//         } else {
//             ++div_fail;
//             ADD_FAILURE() << "[DIV FAIL] a=" << a_str << ", b=" << b_str << ", got=" << quot_bn << ", expected=" << quot_gmp;
//         }
//     }
//     ASSERT_EQ(div_fail, 0) << "Division: " << div_ok << "/" << TEST_CASES << " passed, " << div_fail << " failed.";
// }

// TEST(DivisionManual, DivideByOne) {
//     BigNumber a = make_big_number("123456789");
//     BigNumber b = make_big_number("1");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "123456789");
// }

// TEST(DivisionManual, DivideSelf) {
//     BigNumber a = make_big_number("987654321");
//     BigNumber result = div(a, a);
//     EXPECT_EQ(to_string(result), "1");
// }

// TEST(DivisionManual, DivideZero) {
//     BigNumber a = make_big_number("0");
//     BigNumber b = make_big_number("12345");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "0");
// }

// TEST(DivisionManual, SimpleDivision) {
//     BigNumber a = make_big_number("100");
//     BigNumber b = make_big_number("3");
//     BigNumber result = div(a, b);
//     std::string res_str = to_string(result);
//     EXPECT_TRUE(res_str.substr(0, 2) == "33");
// }

// TEST(DivisionManual, NegativeDivision) {
//     BigNumber a = make_big_number("-50");
//     BigNumber b = make_big_number("2");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "-25");
// }
// TEST(DivisionManual, x1) {
//     BigNumber a = make_big_number("33");
//     BigNumber b = make_big_number("3");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "11");
// }
// TEST(DivisionManual, x2) {
//     BigNumber a = make_big_number("20");
//     BigNumber b = make_big_number("2");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "10");
// }
// TEST(DivisionManual, x3) {
//     BigNumber a = make_big_number("5");
//     BigNumber b = make_big_number("2");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "2.5");
// }

// TEST(DivisionManual, x4) {
//     BigNumber a = make_big_number("50");
//     BigNumber b = make_big_number("2");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "25");
// }

// TEST(DivisionManual, DivideByMinusOne) {
//     BigNumber a = make_big_number("123456789");
//     BigNumber b = make_big_number("-1");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "-123456789");
// }

// TEST(DivisionManual, NegativeDividedByNegative) {
//     BigNumber a = make_big_number("-100");
//     BigNumber b = make_big_number("-10");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "10");
// }

// TEST(DivisionManual, BigIntBySmallInt) {
//     std::string big(100, '9');
//     BigNumber a = make_big_number(big);
//     BigNumber b = make_big_number("3");
//     BigNumber result = div(a, b);
//     mpf_class a_gmp(big, 128);
//     mpf_class b_gmp("3", 128);
//     mpf_class result_gmp = a_gmp / b_gmp;
//     mpf_class result_bn(to_string(result), 128);
//     ASSERT_NEAR(result_bn.get_d(), result_gmp.get_d(), 1e40);
// }

// TEST(DivisionManual, SmallByBig) {
//     BigNumber a = make_big_number("1");
//     std::string big(100, '9');
//     BigNumber b = make_big_number(big);
//     BigNumber result = div(a, b);
//     mpf_class a_gmp("1", 128);
//     mpf_class b_gmp(big, 128);
//     mpf_class result_gmp = a_gmp / b_gmp;
//     mpf_class result_bn(to_string(result), 128);
//     ASSERT_NEAR(result_bn.get_d(), result_gmp.get_d(), 1e-90);
// }

// TEST(DivisionManual, FloatByFloat) {
//     BigNumber a = make_big_number("1.2345");
//     BigNumber b = make_big_number("0.001");
//     BigNumber result = div(a, b);
//     mpf_class a_gmp("1.2345", 128);
//     mpf_class b_gmp("0.001", 128);
//     mpf_class result_gmp = a_gmp / b_gmp;
//     mpf_class result_bn(to_string(result), 128);
//     ASSERT_NEAR(result_bn.get_d(), result_gmp.get_d(), 1e-10);
// }

// TEST(DivisionManual, ZeroDividedByAnything) {
//     BigNumber a = make_big_number("0");
//     BigNumber b = make_big_number("-999999999");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "0");
// }

// TEST(DivisionManual, DivideByFraction) {
//     BigNumber a = make_big_number("1");
//     BigNumber b = make_big_number("0.5");
//     BigNumber result = div(a, b);
//     mpf_class a_gmp("1", 128);
//     mpf_class b_gmp("0.5", 128);
//     mpf_class result_gmp = a_gmp / b_gmp;
//     mpf_class result_bn(to_string(result), 128);
//     ASSERT_NEAR(result_bn.get_d(), result_gmp.get_d(), 1e-10);
// }

// TEST(DivisionManual, DivideByVerySmall) {
//     BigNumber a = make_big_number("1");
//     BigNumber b = make_big_number("0.000000000000000001");
//     BigNumber result = div(a, b);
//     mpf_class a_gmp("1", 128);
//     mpf_class b_gmp("0.000000000000000001", 128);
//     mpf_class result_gmp = a_gmp / b_gmp;
//     mpf_class result_bn(to_string(result), 128);
//     ASSERT_NEAR(result_bn.get_d(), result_gmp.get_d(), 1e8);
// }

// TEST(DivisionManual, DivideNegativeByFraction) {
//     BigNumber a = make_big_number("-1");
//     BigNumber b = make_big_number("0.5");
//     BigNumber result = div(a, b);
//     mpf_class a_gmp("-1", 128);
//     mpf_class b_gmp("0.5", 128);
//     mpf_class result_gmp = a_gmp / b_gmp;
//     mpf_class result_bn(to_string(result), 128);
//     ASSERT_NEAR(result_bn.get_d(), result_gmp.get_d(), 1e-10);
// }

// TEST(DivisionManual, DivideByNegativeFraction) {
//     BigNumber a = make_big_number("1");
//     BigNumber b = make_big_number("-0.5");
//     BigNumber result = div(a, b);
//     mpf_class a_gmp("1", 128);
//     mpf_class b_gmp("-0.5", 128);
//     mpf_class result_gmp = a_gmp / b_gmp;
//     mpf_class result_bn(to_string(result), 128);
//     ASSERT_NEAR(result_bn.get_d(), result_gmp.get_d(), 1e-10);
// }

// TEST(DivisionManual, DivideByItselfFraction) {
//     BigNumber a = make_big_number("0.000123");
//     BigNumber result = div(a, a);
//     EXPECT_EQ(to_string(result), "1");
// }
