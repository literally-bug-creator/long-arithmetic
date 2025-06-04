#include <gtest/gtest.h>
#include "big_number.hpp"
#include "randomString.h"
#include <gmpxx.h>
#include <cmath>
#include <string>

using namespace std;

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
        mpf_class a_gmp(a_str, 128);
        mpf_class b_gmp(b_str, 128);
        mpf_class prod_gmp = a_gmp * b_gmp;
        mpf_class prod_bn(to_string(prod), 128);
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
