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
