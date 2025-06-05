#include <gtest/gtest.h>
#include "big_number.hpp"
#include "randomString.h"
#include <gmpxx.h>
#include <cmath>
#include <string>

using namespace std;

constexpr int TEST_CASES = 10000;
constexpr double EPS = 1e-6;

TEST(Addition, RandomFloatStrings) {
    int add_ok = 0, add_fail = 0;
    for (int i = 0; i < TEST_CASES; ++i) {
        std::string a_str = randomFloatString();
        std::string b_str = randomFloatString();
        BigNumber a = make_big_number(a_str);
        BigNumber b = make_big_number(b_str);
        BigNumber sum = add(a, b);
        mpf_class a_gmp(a_str, 128);
        mpf_class b_gmp(b_str, 128);
        mpf_class sum_gmp = a_gmp + b_gmp;
        mpf_class sum_bn(to_string(sum), 128);
        if (abs(sum_bn - sum_gmp) < EPS) {
            ++add_ok;
        } else {
            ++add_fail;
            ADD_FAILURE() << "[ADD FAIL] a=" << a_str << ", b=" << b_str << ", got=" << sum_bn << ", expected=" << sum_gmp;
        }
    }
    ASSERT_EQ(add_fail, 0) << "Addition: " << add_ok << "/" << TEST_CASES << " passed, " << add_fail << " failed.";
}

TEST(Addition, Manual_BigPlusOne) {
    std::string big(1000, '9');
    BigNumber a = make_big_number(big);
    BigNumber b = make_big_number("1");
    BigNumber sum = add(a, b);
    mpz_class a_gmp(big);
    mpz_class b_gmp("1");
    mpz_class sum_gmp = a_gmp + b_gmp;
    ASSERT_EQ(to_string(sum), sum_gmp.get_str());
}

TEST(Addition, Manual_DifferentLength) {
    BigNumber a = make_big_number("12345678901234567890");
    BigNumber b = make_big_number("987654321");
    BigNumber sum = add(a, b);
    mpz_class a_gmp("12345678901234567890");
    mpz_class b_gmp("987654321");
    mpz_class sum_gmp = a_gmp + b_gmp;
    ASSERT_EQ(to_string(sum), sum_gmp.get_str());
}

TEST(Addition, Manual_PositiveNegative) {
    BigNumber a = make_big_number("1000000000000000000");
    BigNumber b = make_big_number("-999999999999999999");
    BigNumber sum = add(a, b);
    mpz_class a_gmp("1000000000000000000");
    mpz_class b_gmp("-999999999999999999");
    mpz_class sum_gmp = a_gmp + b_gmp;
    ASSERT_EQ(to_string(sum), sum_gmp.get_str());
}

TEST(Addition, Manual_Zero) {
    BigNumber a = make_big_number("0");
    BigNumber b = make_big_number("123456789");
    BigNumber sum = add(a, b);
    ASSERT_EQ(to_string(sum), "123456789");
}

TEST(Addition, Manual_BothNegative) {
    BigNumber a = make_big_number("-123456789");
    BigNumber b = make_big_number("-987654321");
    BigNumber sum = add(a, b);
    mpz_class a_gmp("-123456789");
    mpz_class b_gmp("-987654321");
    mpz_class sum_gmp = a_gmp + b_gmp;
    ASSERT_EQ(to_string(sum), sum_gmp.get_str());
}

TEST(Addition, Manual_FloatEdge) {
    BigNumber a = make_big_number("0.999999999999999999");
    BigNumber b = make_big_number("0.000000000000000001");
    BigNumber sum = add(a, b);
    mpf_class a_gmp("0.999999999999999999", 128);
    mpf_class b_gmp("0.000000000000000001", 128);
    mpf_class sum_gmp = a_gmp + b_gmp;
    mpf_class sum_bn(to_string(sum), 128);
    ASSERT_NEAR(sum_bn.get_d(), sum_gmp.get_d(), 1e-15);
}

TEST(Addition, Manual_DifferentExponent) {
    BigNumber a = make_big_number("1e20");
    BigNumber b = make_big_number("1");
    BigNumber sum = add(a, b);
    mpf_class a_gmp("1e20", 128);
    mpf_class b_gmp("1", 128);
    mpf_class sum_gmp = a_gmp + b_gmp;
    mpf_class sum_bn(to_string(sum), 128);
    ASSERT_NEAR(sum_bn.get_d(), sum_gmp.get_d(), 1e5);
}

TEST(Addition, Manual_BigNegativePlusBigPositive) {
    std::string big_pos(500, '9');
    std::string big_neg = "-" + std::string(500, '9');
    BigNumber a = make_big_number(big_pos);
    BigNumber b = make_big_number(big_neg);
    BigNumber sum = add(a, b);
    mpz_class a_gmp(big_pos);
    mpz_class b_gmp(big_neg);
    mpz_class sum_gmp = a_gmp + b_gmp;
    ASSERT_EQ(to_string(sum), sum_gmp.get_str());
}

TEST(Addition, Manual_CarryPropagation) {
    BigNumber a = make_big_number("999999999999999999");
    BigNumber b = make_big_number("1");
    BigNumber sum = add(a, b);
    mpz_class a_gmp("999999999999999999");
    mpz_class b_gmp("1");
    mpz_class sum_gmp = a_gmp + b_gmp;
    ASSERT_EQ(to_string(sum), sum_gmp.get_str());
}

TEST(Addition, Manual_MinusOnePlusOne) {
    BigNumber a = make_big_number("-1");
    BigNumber b = make_big_number("1");
    BigNumber sum = add(a, b);
    ASSERT_EQ(to_string(sum), "0");
}

TEST(Addition, Manual_FractionalCarry) {
    BigNumber a = make_big_number("0.999999999999999999");
    BigNumber b = make_big_number("0.000000000000000001");
    BigNumber sum = add(a, b);
    mpf_class a_gmp("0.999999999999999999", 128);
    mpf_class b_gmp("0.000000000000000001", 128);
    mpf_class sum_gmp = a_gmp + b_gmp;
    mpf_class sum_bn(to_string(sum), 128);
    ASSERT_NEAR(sum_bn.get_d(), sum_gmp.get_d(), 1e-15);
}

TEST(Addition, Manual_LargeExponentDifference) {
    BigNumber a = make_big_number("1e100");
    BigNumber b = make_big_number("1");
    BigNumber sum = add(a, b);
    mpf_class a_gmp("1e100", 128);
    mpf_class b_gmp("1", 128);
    mpf_class sum_gmp = a_gmp + b_gmp;
    mpf_class sum_bn(to_string(sum), 128);
    ASSERT_NEAR(sum_bn.get_d(), sum_gmp.get_d(), 1e85);
}

TEST(Addition, Manual_SmallFractionalSum) {
    BigNumber a = make_big_number("0.000000000000000001");
    BigNumber b = make_big_number("0.000000000000000002");
    BigNumber sum = add(a, b);
    mpf_class a_gmp("0.000000000000000001", 128);
    mpf_class b_gmp("0.000000000000000002", 128);
    mpf_class sum_gmp = a_gmp + b_gmp;
    mpf_class sum_bn(to_string(sum), 128);
    ASSERT_NEAR(sum_bn.get_d(), sum_gmp.get_d(), 1e-18);
}

TEST(Addition, Manual_ZeroPlusZero) {
    BigNumber a = make_big_number("0");
    BigNumber b = make_big_number("0");
    BigNumber sum = add(a, b);
    ASSERT_EQ(to_string(sum), "0");
}

TEST(Addition, Manual_BigFloatPlusBigInt) {
    BigNumber a = make_big_number("999999999999999.999999999999999");
    BigNumber b = make_big_number("1");
    BigNumber sum = add(a, b);
    mpf_class a_gmp("999999999999999.999999999999999", 128);
    mpf_class b_gmp("1", 128);
    mpf_class sum_gmp = a_gmp + b_gmp;
    mpf_class sum_bn(to_string(sum), 128);
    ASSERT_NEAR(sum_bn.get_d(), sum_gmp.get_d(), 1e-6);
}

TEST(Addition, Manual_NegativeFloatPlusPositiveInt) {
    BigNumber a = make_big_number("-12345.6789");
    BigNumber b = make_big_number("12345");
    BigNumber sum = add(a, b);
    mpf_class a_gmp("-12345.6789", 128);
    mpf_class b_gmp("12345", 128);
    mpf_class sum_gmp = a_gmp + b_gmp;
    mpf_class sum_bn(to_string(sum), 128);
    ASSERT_NEAR(sum_bn.get_d(), sum_gmp.get_d(), 1e-6);
}
