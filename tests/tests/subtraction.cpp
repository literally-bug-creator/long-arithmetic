#include <gtest/gtest.h>
#include "big_number.hpp"
#include "randomString.h"
#include <gmpxx.h>
#include <cmath>
#include <string>

using namespace std;

constexpr int TEST_CASES = 10000;
constexpr double EPS = 1e-6;

TEST(Subtraction, RandomFloatStrings) {
    int sub_ok = 0, sub_fail = 0;
    for (int i = 0; i < TEST_CASES; ++i) {
        std::string a_str = randomFloatString();
        std::string b_str = randomFloatString();
        BigNumber a = make_big_number(a_str);
        BigNumber b = make_big_number(b_str);
        BigNumber diff = sub(a, b);
        mpf_class a_gmp(a_str, 128);
        mpf_class b_gmp(b_str, 128);
        mpf_class diff_gmp = a_gmp - b_gmp;
        mpf_class diff_bn(to_string(diff), 128);
        if (abs(diff_bn - diff_gmp) < EPS) {
            ++sub_ok;
        } else {
            ++sub_fail;
            ADD_FAILURE() << "[SUB FAIL] a=" << a_str << ", b=" << b_str << ", got=" << diff_bn << ", expected=" << diff_gmp;
        }
    }
    ASSERT_EQ(sub_fail, 0) << "Subtraction: " << sub_ok << "/" << TEST_CASES << " passed, " << sub_fail << " failed.";
}

TEST(Subtraction, Manual_BigMinusOne) {
    std::string big(1000, '9');
    BigNumber a = make_big_number(big);
    BigNumber b = make_big_number("1");
    BigNumber diff = sub(a, b);
    mpz_class a_gmp(big);
    mpz_class b_gmp("1");
    mpz_class diff_gmp = a_gmp - b_gmp;
    ASSERT_EQ(to_string(diff), diff_gmp.get_str());
}

TEST(Subtraction, Manual_DifferentLength) {
    BigNumber a = make_big_number("12345678901234567890");
    BigNumber b = make_big_number("987654321");
    BigNumber diff = sub(a, b);
    mpz_class a_gmp("12345678901234567890");
    mpz_class b_gmp("987654321");
    mpz_class diff_gmp = a_gmp - b_gmp;
    ASSERT_EQ(to_string(diff), diff_gmp.get_str());
}

TEST(Subtraction, Manual_PositiveNegative) {
    BigNumber a = make_big_number("1000000000000000000");
    BigNumber b = make_big_number("-999999999999999999");
    BigNumber diff = sub(a, b);
    mpz_class a_gmp("1000000000000000000");
    mpz_class b_gmp("-999999999999999999");
    mpz_class diff_gmp = a_gmp - b_gmp;
    ASSERT_EQ(to_string(diff), diff_gmp.get_str());
}

TEST(Subtraction, Manual_Zero) {
    BigNumber a = make_big_number("0");
    BigNumber b = make_big_number("123456789");
    BigNumber diff = sub(a, b);
    mpz_class a_gmp("0");
    mpz_class b_gmp("123456789");
    mpz_class diff_gmp = a_gmp - b_gmp;
    ASSERT_EQ(to_string(diff), diff_gmp.get_str());
}

TEST(Subtraction, Manual_BothNegative) {
    BigNumber a = make_big_number("-123456789");
    BigNumber b = make_big_number("-987654321");
    BigNumber diff = sub(a, b);
    mpz_class a_gmp("-123456789");
    mpz_class b_gmp("-987654321");
    mpz_class diff_gmp = a_gmp - b_gmp;
    ASSERT_EQ(to_string(diff), diff_gmp.get_str());
}

TEST(Subtraction, Manual_FloatEdge) {
    BigNumber a = make_big_number("0.999999999999999999");
    BigNumber b = make_big_number("0.000000000000000001");
    BigNumber diff = sub(a, b);
    mpf_class a_gmp("0.999999999999999999", 128);
    mpf_class b_gmp("0.000000000000000001", 128);
    mpf_class diff_gmp = a_gmp - b_gmp;
    mpf_class diff_bn(to_string(diff), 128);
    ASSERT_NEAR(diff_bn.get_d(), diff_gmp.get_d(), 1e-15);
}

TEST(Subtraction, Manual_DifferentExponent) {
    BigNumber a = make_big_number("1e20");
    BigNumber b = make_big_number("1");
    BigNumber diff = sub(a, b);
    mpf_class a_gmp("1e20", 128);
    mpf_class b_gmp("1", 128);
    mpf_class diff_gmp = a_gmp - b_gmp;
    mpf_class diff_bn(to_string(diff), 128);
    ASSERT_NEAR(diff_bn.get_d(), diff_gmp.get_d(), 1e5);
}

TEST(Subtraction, Manual_LeadingZeros) {
    BigNumber a = make_big_number("0000123");
    BigNumber b = make_big_number("0000045");
    BigNumber diff = sub(a, b);
    ASSERT_EQ(to_string(diff), "78");
}

TEST(Subtraction, Manual_BigNegativeMinusBigPositive) {
    std::string big_pos(500, '9');
    std::string big_neg = "-" + std::string(500, '9');
    BigNumber a = make_big_number(big_neg);
    BigNumber b = make_big_number(big_pos);
    BigNumber diff = sub(a, b);
    mpz_class a_gmp(big_neg);
    mpz_class b_gmp(big_pos);
    mpz_class diff_gmp = a_gmp - b_gmp;
    ASSERT_EQ(to_string(diff), diff_gmp.get_str());
}

TEST(Subtraction, Manual_CarryPropagation) {
    BigNumber a = make_big_number("1000000000000000000");
    BigNumber b = make_big_number("1");
    BigNumber diff = sub(a, b);
    mpz_class a_gmp("1000000000000000000");
    mpz_class b_gmp("1");
    mpz_class diff_gmp = a_gmp - b_gmp;
    ASSERT_EQ(to_string(diff), diff_gmp.get_str());
}

TEST(Subtraction, Manual_MinusOneMinusOne) {
    BigNumber a = make_big_number("-1");
    BigNumber b = make_big_number("1");
    BigNumber diff = sub(a, b);
    ASSERT_EQ(to_string(diff), "-2");
}

TEST(Subtraction, Manual_FractionalBorrow) {
    BigNumber a = make_big_number("1.000000000000000000");
    BigNumber b = make_big_number("0.000000000000000001");
    BigNumber diff = sub(a, b);
    mpf_class a_gmp("1.000000000000000000", 128);
    mpf_class b_gmp("0.000000000000000001", 128);
    mpf_class diff_gmp = a_gmp - b_gmp;
    mpf_class diff_bn(to_string(diff), 128);
    ASSERT_NEAR(diff_bn.get_d(), diff_gmp.get_d(), 1e-15);
}

TEST(Subtraction, Manual_LargeExponentDifference) {
    BigNumber a = make_big_number("1e100");
    BigNumber b = make_big_number("1");
    BigNumber diff = sub(a, b);
    mpf_class a_gmp("1e100", 128);
    mpf_class b_gmp("1", 128);
    mpf_class diff_gmp = a_gmp - b_gmp;
    mpf_class diff_bn(to_string(diff), 128);
    ASSERT_NEAR(diff_bn.get_d(), diff_gmp.get_d(), 1e85);
}

TEST(Subtraction, Manual_SmallFractionalDiff) {
    BigNumber a = make_big_number("0.000000000000000003");
    BigNumber b = make_big_number("0.000000000000000002");
    BigNumber diff = sub(a, b);
    mpf_class a_gmp("0.000000000000000003", 128);
    mpf_class b_gmp("0.000000000000000002", 128);
    mpf_class diff_gmp = a_gmp - b_gmp;
    mpf_class diff_bn(to_string(diff), 128);
    ASSERT_NEAR(diff_bn.get_d(), diff_gmp.get_d(), 1e-18);
}

TEST(Subtraction, Manual_ZeroMinusZero) {
    BigNumber a = make_big_number("0");
    BigNumber b = make_big_number("0");
    BigNumber diff = sub(a, b);
    ASSERT_EQ(to_string(diff), "0");
}

TEST(Subtraction, Manual_BigFloatMinusBigInt) {
    BigNumber a = make_big_number("999999999999999.999999999999999");
    BigNumber b = make_big_number("1");
    BigNumber diff = sub(a, b);
    mpf_class a_gmp("999999999999999.999999999999999", 128);
    mpf_class b_gmp("1", 128);
    mpf_class diff_gmp = a_gmp - b_gmp;
    mpf_class diff_bn(to_string(diff), 128);
    ASSERT_NEAR(diff_bn.get_d(), diff_gmp.get_d(), 1e-6);
}

TEST(Subtraction, Manual_NegativeFloatMinusPositiveInt) {
    BigNumber a = make_big_number("-12345.6789");
    BigNumber b = make_big_number("12345");
    BigNumber diff = sub(a, b);
    mpf_class a_gmp("-12345.6789", 128);
    mpf_class b_gmp("12345", 128);
    mpf_class diff_gmp = a_gmp - b_gmp;
    mpf_class diff_bn(to_string(diff), 128);
    ASSERT_NEAR(diff_bn.get_d(), diff_gmp.get_d(), 1e-6);
}
