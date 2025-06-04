#include <gtest/gtest.h>
#include "big_number.hpp"
#include "randomString.h"
#include <gmpxx.h>
#include <cmath>
#include <string>

using namespace std;

constexpr int TEST_CASES = 10000;
constexpr double EPS = 1e-6;

TEST(Division, RandomFloatStrings) {
    int div_ok = 0, div_fail = 0;
    for (int i = 0; i < TEST_CASES; ++i) {
        std::string a_str = randomFloatString();
        std::string b_str = randomFloatString();
        if (b_str == "0" || b_str == "0.0" || b_str == "-0" || b_str == "-0.0") {
            --i;
            continue;
        }
        BigNumber a = make_big_number(a_str);
        BigNumber b = make_big_number(b_str);
        BigNumber quot = div(a, b);
        mpf_class a_gmp(a_str, 128);
        mpf_class b_gmp(b_str, 128);
        mpf_class quot_gmp = a_gmp / b_gmp;
        mpf_class quot_bn(to_string(quot), 128);
        if (abs(quot_bn - quot_gmp) < EPS) {
            ++div_ok;
        } else {
            ++div_fail;
            ADD_FAILURE() << "[DIV FAIL] a=" << a_str << ", b=" << b_str << ", got=" << quot_bn << ", expected=" << quot_gmp;
        }
    }
    ASSERT_EQ(div_fail, 0) << "Division: " << div_ok << "/" << TEST_CASES << " passed, " << div_fail << " failed.";
}
