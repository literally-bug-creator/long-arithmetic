#include <gtest/gtest.h>
#include "big_number.hpp"
#include "randomString.h"
#include <gmpxx.h>
#include <cmath>
#include <string>

using namespace std;

constexpr int TEST_CASES = 10000;
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
