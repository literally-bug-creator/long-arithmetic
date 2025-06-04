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
