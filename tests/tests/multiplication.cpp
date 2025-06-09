#include <gtest/gtest.h>
#include "big_number.hpp"
#include "randomString.h"
#include <gmpxx.h>
#include <cmath>
#include <string>

using namespace std;
using namespace big_number;

constexpr int TEST_CASES = 10000;
constexpr double EPS = 1e-6;

TEST(Multiplication, RandomFloatStrings) {
    int mul_ok = 0, mul_fail = 0;
    for (int i = 0; i < TEST_CASES; ++i) {
        std::string a_str = randomFloatString();
        std::string b_str = randomFloatString();
        BigNumber a = make_big_number(a_str);
        BigNumber b = make_big_number(b_str);
        BigNumber prod = mul(a, b);
        mpf_class a_gmp(a_str, 1280);
        mpf_class b_gmp(b_str, 1280);
        mpf_class prod_gmp = a_gmp * b_gmp;
        mpf_class prod_bn(to_string(prod), 1280);
        if (abs(prod_bn - prod_gmp) < EPS) {
            ++mul_ok;
        } else {
            ++mul_fail;
            ADD_FAILURE() << "[MUL FAIL] a=" << a_str << ", b=" << b_str << ", got=" << prod_bn << ", expected=" << prod_gmp;
            std::cout << a.exponent << " " << b.exponent << std::endl;
            std::cout << prod.exponent << std::endl;
        }
    }
    ASSERT_EQ(mul_fail, 0) << "Multiplication: " << mul_ok << "/" << TEST_CASES << " passed, " << mul_fail << " failed.";
}

TEST(Multiplication, Manual_1000x1000_Nines) {
    std::string nines_1000(10000, '9');
    BigNumber a = make_big_number(nines_1000);
    BigNumber b = make_big_number(nines_1000);
    BigNumber prod = mul(a, b);
    mpz_class a_gmp(nines_1000);
    mpz_class b_gmp(nines_1000);
    mpz_class prod_gmp = a_gmp * b_gmp;
    ASSERT_EQ(to_string(prod), prod_gmp.get_str()) << "Manual 1000x1000 nines failed: got=" << to_string(prod) << ", expected=" << prod_gmp.get_str();
}

TEST(Multiplication, Manual_BigTimesOne) {
    std::string big(1000, '9');
    BigNumber a = make_big_number(big);
    BigNumber b = make_big_number("1");
    BigNumber prod = mul(a, b);
    mpz_class a_gmp(big);
    mpz_class b_gmp("1");
    mpz_class prod_gmp = a_gmp * b_gmp;
    ASSERT_EQ(to_string(prod), prod_gmp.get_str());
}

TEST(Multiplication, Manual_DifferentLength) {
    BigNumber a = make_big_number("12345678901234567890");
    BigNumber b = make_big_number("987654321");
    BigNumber prod = mul(a, b);
    mpz_class a_gmp("12345678901234567890");
    mpz_class b_gmp("987654321");
    mpz_class prod_gmp = a_gmp * b_gmp;
    ASSERT_EQ(to_string(prod), prod_gmp.get_str());
}

TEST(Multiplication, Manual_PositiveNegative) {
    BigNumber a = make_big_number("1000000000000000000");
    BigNumber b = make_big_number("-999999999999999999");
    BigNumber prod = mul(a, b);
    mpz_class a_gmp("1000000000000000000");
    mpz_class b_gmp("-999999999999999999");
    mpz_class prod_gmp = a_gmp * b_gmp;
    ASSERT_EQ(to_string(prod), prod_gmp.get_str());
}

TEST(Multiplication, Manual_Zero) {
    BigNumber a = make_big_number("0");
    BigNumber b = make_big_number("123456789");
    BigNumber prod = mul(a, b);
    ASSERT_EQ(to_string(prod), "0");
}

TEST(Multiplication, Manual_BothNegative) {
    BigNumber a = make_big_number("-123456789");
    BigNumber b = make_big_number("-987654321");
    BigNumber prod = mul(a, b);
    mpz_class a_gmp("-123456789");
    mpz_class b_gmp("-987654321");
    mpz_class prod_gmp = a_gmp * b_gmp;
    ASSERT_EQ(to_string(prod), prod_gmp.get_str());
}

TEST(Multiplication, Manual_FloatEdge) {
    BigNumber a = make_big_number("0.999999999999999999");
    BigNumber b = make_big_number("0.000000000000000001");
    BigNumber prod = mul(a, b);
    mpf_class a_gmp("0.999999999999999999", 128);
    mpf_class b_gmp("0.000000000000000001", 128);
    mpf_class prod_gmp = a_gmp * b_gmp;
    mpf_class prod_bn(to_string(prod), 128);
    ASSERT_NEAR(prod_bn.get_d(), prod_gmp.get_d(), 1e-15);
}

TEST(Multiplication, Manual_DifferentExponent) {
    BigNumber a = make_big_number("1e20");
    BigNumber b = make_big_number("1e-10");
    BigNumber prod = mul(a, b);
    mpf_class a_gmp("1e20", 128);
    mpf_class b_gmp("1e-10", 128);
    mpf_class prod_gmp = a_gmp * b_gmp;
    mpf_class prod_bn(to_string(prod), 128);
    ASSERT_NEAR(prod_bn.get_d(), prod_gmp.get_d(), 1e5);
}

TEST(Multiplication, Manual_BigNegativeTimesBigPositive) {
    std::string big_pos(500, '9');
    std::string big_neg = "-" + std::string(500, '9');
    BigNumber a = make_big_number(big_neg);
    BigNumber b = make_big_number(big_pos);
    BigNumber prod = mul(a, b);
    mpz_class a_gmp(big_neg);
    mpz_class b_gmp(big_pos);
    mpz_class prod_gmp = a_gmp * b_gmp;
    ASSERT_EQ(to_string(prod), prod_gmp.get_str());
}

TEST(Multiplication, Manual_OneTimesMinusOne) {
    BigNumber a = make_big_number("1");
    BigNumber b = make_big_number("-1");
    BigNumber prod = mul(a, b);
    ASSERT_EQ(to_string(prod), "-1");
}

TEST(Multiplication, Manual_BigFloatTimesBigInt) {
    BigNumber a = make_big_number("999999999999999.999999999999999");
    BigNumber b = make_big_number("1");
    BigNumber prod = mul(a, b);
    mpf_class a_gmp("999999999999999.999999999999999", 128);
    mpf_class b_gmp("1", 128);
    mpf_class prod_gmp = a_gmp * b_gmp;
    mpf_class prod_bn(to_string(prod), 128);
    ASSERT_NEAR(prod_bn.get_d(), prod_gmp.get_d(), 1e-6);
}

TEST(Multiplication, Manual_NegativeFloatTimesPositiveInt) {
    BigNumber a = make_big_number("-12345.6789");
    BigNumber b = make_big_number("12345");
    BigNumber prod = mul(a, b);
    mpf_class a_gmp("-12345.6789", 128);
    mpf_class b_gmp("12345", 128);
    mpf_class prod_gmp = a_gmp * b_gmp;
    mpf_class prod_bn(to_string(prod), 128);
    ASSERT_NEAR(prod_bn.get_d(), prod_gmp.get_d(), 1e-6);
}
