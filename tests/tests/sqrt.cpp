#include <gtest/gtest.h>
#include "big_number.hpp"
#include "randomString.h"
#include <gmpxx.h>
#include <cmath>
#include <string>

using namespace std;
using namespace big_number;

constexpr int TEST_CASES = 100;
constexpr double EPS = 1e-6;

TEST(Sqrt, RandomFloatStrings) {
    int sqrt_ok = 0, sqrt_fail = 0;
    for (int i = 0; i < TEST_CASES; ++i) {
        std::string a_str = randomFloatString();
        mpf_class a_gmp(a_str, 128);
        if (a_gmp < 0) {
            --i;
            continue;
        }
        BigNumber a = make_big_number(a_str);
        BigNumber root = sqrt(a);
        mpf_class root_gmp = sqrt(a_gmp);
        mpf_class root_bn(to_string(root), 128);
        if (abs(root_bn - root_gmp) < EPS) {
            ++sqrt_ok;
        } else {
            ++sqrt_fail;
            ADD_FAILURE() << "[SQRT FAIL] a=" << a_str << ", got=" << root_bn << ", expected=" << root_gmp;
        }
    }
    ASSERT_EQ(sqrt_fail, 0) << "Sqrt: " << sqrt_ok << "/" << TEST_CASES << " passed, " << sqrt_fail << " failed.";
}

TEST(Sqrt, Manual_PerfectSquares) {
    BigNumber a = make_big_number("4");
    BigNumber root = sqrt(a);
    ASSERT_EQ(to_string(root), "2");
    a = make_big_number("9");
    root = sqrt(a);
    ASSERT_EQ(to_string(root), "3");
    a = make_big_number("1000000");
    root = sqrt(a);
    ASSERT_EQ(to_string(root), "1000");
}

TEST(Sqrt, Manual_FractionalSquares) {
    BigNumber a = make_big_number("0.25");
    BigNumber root = sqrt(a);
    ASSERT_EQ(to_string(root), "0.5");
    a = make_big_number("0.0001");
    root = sqrt(a);
    ASSERT_EQ(to_string(root), "0.01");
}

TEST(Sqrt, Manual_BigInt) {
    std::string big(100000, '9');
    BigNumber a = make_big_number(big);
    BigNumber root = sqrt(a);
    mpf_class a_gmp(big, 128);
    mpf_class root_gmp = sqrt(a_gmp);
    mpf_class root_bn(to_string(root), 128);
    ASSERT_NEAR(root_bn.get_d(), root_gmp.get_d(), 1e40);
}

TEST(Sqrt, Manual_One) {
    BigNumber a = make_big_number("1");
    BigNumber root = sqrt(a);
    ASSERT_EQ(to_string(root), "1");
}

TEST(Sqrt, Manual_Zero) {
    BigNumber a = make_big_number("0");
    BigNumber root = sqrt(a);
    ASSERT_EQ(to_string(root), "0");
}

TEST(Sqrt, Manual_SmallFraction) {
    BigNumber a = make_big_number("0.0000000000000004");
    BigNumber root = sqrt(a);
    mpf_class a_gmp("0.0000000000000004", 128);
    mpf_class root_gmp = sqrt(a_gmp);
    mpf_class root_bn(to_string(root), 128);
    ASSERT_NEAR(root_bn.get_d(), root_gmp.get_d(), 1e-12);
}
