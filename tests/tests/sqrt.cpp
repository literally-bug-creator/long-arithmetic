#include <gtest/gtest.h>
#include "big_number.hpp"
#include "randomString.h"
#include <gmpxx.h>
#include <cmath>
#include <string>

using namespace std;
using namespace big_number;

constexpr int TEST_CASES = 1000;
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

TEST(Sqrt, Manual_PerfectSquares) {
    BigNumber a = make_big_number("4");
    BigNumber root = sqrt(a);
    ASSERT_EQ(to_string(root), "2");
    a = make_big_number("16");
    root = sqrt(a);
    ASSERT_EQ(to_string(root), "4");
    a = make_big_number("0.25");
    root = sqrt(a);
    ASSERT_EQ(to_string(root), "0.5");
    a = make_big_number("9");
    root = sqrt(a);
    ASSERT_EQ(to_string(root), "3");
    a = make_big_number("1000000");
    root = sqrt(a);
    ASSERT_EQ(to_string(root), "1000");
}

TEST(Sqrt, Manual_FractionalSquares) {
    BigNumber a = make_big_number("25");
    BigNumber root = sqrt(a);
    ASSERT_EQ(to_string(root), "5");
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

TEST(Sqrt, Manual_MoreCases) {
    // Test cases with different decimal places
    BigNumber a = make_big_number("2.25");
    BigNumber root = sqrt(a);
    ASSERT_EQ(to_string(root), "1.5");

    a = make_big_number("0.0625");
    root = sqrt(a);
    ASSERT_EQ(to_string(root), "0.25");

    // Test cases with repeating decimals
    a = make_big_number("0.11111111111111111111");
    root = sqrt(a);
    mpf_class a_gmp("0.11111111111111111111", 128);
    mpf_class root_gmp = sqrt(a_gmp);
    mpf_class root_bn(to_string(root), 128);
    ASSERT_NEAR(root_bn.get_d(), root_gmp.get_d(), 1e-10);

    // Test cases with numbers close to 1
    a = make_big_number("0.99999999999999999999");
    root = sqrt(a);
    mpf_class a_gmp3("0.99999999999999999999", 128);
    mpf_class root_gmp3 = sqrt(a_gmp3);
    mpf_class root_bn3(to_string(root), 128);
    ASSERT_NEAR(root_bn3.get_d(), root_gmp3.get_d(), 1e-10);

    // Test cases with large numbers
    a = make_big_number("12345678901234567890");
    root = sqrt(a);
    mpf_class a_gmp4("12345678901234567890", 128);
    mpf_class root_gmp4 = sqrt(a_gmp4);
    mpf_class root_bn4(to_string(root), 128);
    ASSERT_NEAR(root_bn4.get_d(), root_gmp4.get_d(), 1e10);

    a = make_big_number("12");
    root = sqrt(a);
    mpf_class a_gmp5("12", 128);
    mpf_class root_gmp5 = sqrt(a_gmp4);
    mpf_class root_bn5(to_string(root), 128);
    ASSERT_EQ(root_bn4.get_d(), root_gmp4.get_d());
}

TEST(Sqrt, Manual_PowersOfTen) {
    // Test square roots of powers of 10
    std::vector<std::pair<std::string, std::string>> test_cases = {
        {"100", "10"},
        {"10000", "100"},
        {"1000000", "1000"},
        {"0.01", "0.1"},
        {"0.0001", "0.01"},
        {"0.000001", "0.001"}
    };

    for (const auto& [input, expected] : test_cases) {
        BigNumber a = make_big_number(input);
        BigNumber root = sqrt(a);
        ASSERT_EQ(to_string(root), expected) << "Failed for input: " << input;
    }
}

TEST(Sqrt, Manual_CommonFractions) {
    // Test square roots of common fractions
    std::vector<std::pair<std::string, std::string>> test_cases = {
        {"0.25", "0.5"},
        {"0.36", "0.6"},
        {"0.49", "0.7"},
        {"0.64", "0.8"},
        {"0.81", "0.9"},
        {"1.44", "1.2"},
        {"1.69", "1.3"},
        {"1.96", "1.4"},
        {"2.25", "1.5"}
    };

    for (const auto& [input, expected] : test_cases) {
        BigNumber a = make_big_number(input);
        BigNumber root = sqrt(a);
        ASSERT_EQ(to_string(root), expected) << "Failed for input: " << input;
    }
}

TEST(Sqrt, Manual_SpecialNumbers) {
    // Test with numbers close to zero
    BigNumber a = make_big_number("0.0000000001");
    BigNumber root = sqrt(a);
    mpf_class a_gmp2("0.0000000001", 128);
    mpf_class root_gmp2 = sqrt(a_gmp2);
    mpf_class root_bn2(to_string(root), 128);
    ASSERT_NEAR(root_bn2.get_d(), root_gmp2.get_d(), 1e-10);

    // Test with numbers close to 2
    a = make_big_number("2");
    root = sqrt(a);
    mpf_class a_gmp3("2", 128);
    mpf_class root_gmp3 = sqrt(a_gmp3);
    mpf_class root_bn3(to_string(root), 128);
    ASSERT_NEAR(root_bn3.get_d(), root_gmp3.get_d(), 1e-10);
}

TEST(Sqrt, Manual_LargeNumbers) {
    // Test with large numbers
    std::vector<std::string> test_cases = {
        "1000000000000000000000000000000",
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
        ASSERT_NEAR(root_bn.get_d(), root_gmp.get_d(), 1e10) << "Failed for input: " << input;
    }
}

TEST(Sqrt, Manual_PrecisionTest) {
    // Test precision with numbers that have many decimal places
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
    // Test with numbers that have repeating decimal patterns
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

TEST(Sqrt, Manual_MorePerfectSquaresAndFractions) {
    struct TestCase {
        const char* input;
        const char* expected;
    } test_cases[] = {
        {"9", "3"},
        {"81", "9"},
        {"0.81", "0.9"},
        {"0.0001", "0.01"},
        {"0.000001", "0.001"},
        {"0.00000001", "0.0001"},
        {"1e-8", "0.0001"},
        {"1e-4", "0.01"},
        {"1e-2", "0.1"},
        {"1e2", "10"},
        {"1e4", "100"},
        {"1e6", "1000"},
        {"0.0000000009", "0.00003"},
        {"0.0009", "0.03"},
        {"100", "10"},
        {"10000", "100"},
        {"0.00000001", "0.0001"},
    };

    for (const auto& [input, expected] : test_cases) {
        BigNumber a = make_big_number(input);
        BigNumber root = sqrt(a);
        ASSERT_EQ(to_string(root), expected) << "input: " << input;
    }
}
