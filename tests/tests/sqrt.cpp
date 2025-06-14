#include <gtest/gtest.h>
#include "big_number.hpp"
#include "randomString.h"
#include <gmpxx.h>
#include <cmath>
#include <string>

using namespace std;
using namespace big_number;

constexpr int TEST_CASES = 10;
constexpr double EPS = 1e-10;

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



TEST(Sqrt, Manual_BigInt) {
    std::string big(100000, '9');
    BigNumber a = make_big_number(big);
    BigNumber root = sqrt(a);
    mpf_class a_gmp(big, 1280);
    mpf_class root_gmp = sqrt(a_gmp);
    mpf_class root_bn(to_string(root), 1280);
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


TEST(Sqrt, Manual_SpecialNumbers) {

    BigNumber a = make_big_number("0.0000000001");
    BigNumber root = sqrt(a);
    mpf_class a_gmp2("0.0000000001", 128);
    mpf_class root_gmp2 = sqrt(a_gmp2);
    mpf_class root_bn2(to_string(root), 128);
    ASSERT_NEAR(root_bn2.get_d(), root_gmp2.get_d(), 1e-10);


    a = make_big_number("2");
    root = sqrt(a);
    mpf_class a_gmp3("2", 128);
    mpf_class root_gmp3 = sqrt(a_gmp3);
    mpf_class root_bn3(to_string(root), 128);
    ASSERT_NEAR(root_bn3.get_d(), root_gmp3.get_d(), 1e-10);
}

TEST(Sqrt, Manual_LargeNumbers) {

    std::vector<std::string> test_cases = {
        "999999999999999999999999999999",
        "123456789012345678901234567890",
        "987654321098765432109876543210"
    };

    for (const auto& input : test_cases) {
        BigNumber a = make_big_number(input);
        BigNumber root = sqrt(a);
        mpf_class a_gmp(input, 128);
        mpf_class root_gmp = sqrt(a_gmp);
        mpf_class root_bn(to_string(root), 128);
        ASSERT_NEAR(root_bn.get_d(), root_gmp.get_d(), 1e20) << "Failed for input: " << input;
    }
}

TEST(Sqrt, Manual_PrecisionTest) {

    std::vector<std::pair<std::string, std::string>> test_cases = {
        {"2.000000000000000000000000000000", "1.414213562373095048801688724209"},
        {"3.000000000000000000000000000000", "1.732050807568877293527446341505"},
        {"5.000000000000000000000000000000", "2.236067977499789696409173668731"},
        {"7.000000000000000000000000000000", "2.645751311064590590501615753639"}
    };

    for (const auto& [input, expected] : test_cases) {
        BigNumber a = make_big_number(input);
        BigNumber root = sqrt(a);
        mpf_class a_gmp(input, 128);
        mpf_class root_gmp = sqrt(a_gmp);
        mpf_class root_bn(to_string(root), 128);
        ASSERT_NEAR(root_bn.get_d(), root_gmp.get_d(), 1e-10) << "Failed for input: " << input;
    }
}

TEST(Sqrt, Manual_RepeatingDecimals) {

    std::vector<std::string> test_cases = {
        "0.333333333333333333333333333333",
        "0.666666666666666666666666666666",
        "0.999999999999999999999999999999",
        "1.333333333333333333333333333333"
    };

    for (const auto& input : test_cases) {
        BigNumber a = make_big_number(input);
        BigNumber root = sqrt(a);
        mpf_class a_gmp(input, 128);
        mpf_class root_gmp = sqrt(a_gmp);
        mpf_class root_bn(to_string(root), 128);
        ASSERT_NEAR(root_bn.get_d(), root_gmp.get_d(), 1e-10) << "Failed for input: " << input;
    }
}


