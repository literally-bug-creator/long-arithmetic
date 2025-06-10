#include <gtest/gtest.h>
#include "big_number.hpp"
#include "randomString.h"

using namespace std;
using namespace big_number;

TEST(Constructor, RandomFloatStrings) {
    for (int i = 0; i < 10000; ++i) {
        std::string rand_str = randomFloatString();
        BigNumber number = make_big_number(rand_str);
        std::string number_str = to_string(number);
        BigNumber number2 = make_big_number(number_str);
        ASSERT_TRUE(is_equal(number, number2)) << "Failed on input: " << rand_str << ", to_string: " << number_str;
    }
}

TEST(Constructor, ManualEdgeCases) {
    std::string big_int = "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
    BigNumber n1 = make_big_number(big_int);
    ASSERT_EQ(to_string(n1), big_int);

    std::string big_neg = "-123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
    BigNumber n2 = make_big_number(big_neg);
    ASSERT_EQ(to_string(n2), big_neg);

    std::string big_frac = "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890.123456789";
    BigNumber n3 = make_big_number(big_frac);
    ASSERT_EQ(to_string(n3), big_frac);

    BigNumber n4 = make_big_number("0");
    ASSERT_EQ(to_string(n4), "0");

    BigNumber n6 = make_big_number("0.123");
    ASSERT_EQ(to_string(n6), "0.123");

    BigNumber n7 = make_big_number(".456");
    ASSERT_EQ(to_string(n7), "0.456");

    BigNumber n12 = make_big_number("0.123456789123456789123456789");
    ASSERT_EQ(to_string(n12), "0.123456789123456789123456789");

    std::string big_int2 = std::string(99999, '9');
    BigNumber n13 = make_big_number(big_int2);
    ASSERT_EQ(to_string(n13), big_int2);

    BigNumber n14 = make_big_number("123.0000");
    ASSERT_EQ(to_string(n14), "123");

    BigNumber n15 = make_big_number("0.0000");
    ASSERT_EQ(to_string(n15), "0");

    BigNumber n16 = make_big_number("-1.");
    ASSERT_EQ(to_string(n16), "-1");

    BigNumber n17 = make_big_number(".1");
    ASSERT_EQ(to_string(n17), "0.1");

    BigNumber n18 = make_big_number("-.1");
    ASSERT_EQ(to_string(n18), "-0.1");

    BigNumber n19 = make_big_number("0.");
    ASSERT_EQ(to_string(n19), "0");

    BigNumber n20 = make_big_number(".0");
    ASSERT_EQ(to_string(n20), "0");

    BigNumber n0 = make_big_number("-0");
    ASSERT_EQ(to_string(n0), "0");

    BigNumber n21 = make_big_number("1.23e3");
    ASSERT_EQ(to_string(n21), "1230");
    BigNumber n22 = make_big_number("1.23E3");
    ASSERT_EQ(to_string(n22), "1230");
    BigNumber n23 = make_big_number("1.23e+3");
    ASSERT_EQ(to_string(n23), "1230");
    BigNumber n24 = make_big_number("1.23e2");
    ASSERT_EQ(to_string(n24), "123");
    BigNumber n25 = make_big_number("1.23e0");
    ASSERT_EQ(to_string(n25), "1.23");

    BigNumber n26 = make_big_number("1.23e-2");
    ASSERT_EQ(to_string(n26), "0.0123");
    BigNumber n27 = make_big_number("123e-2");
    ASSERT_EQ(to_string(n27), "1.23");
    BigNumber n28 = make_big_number("123e-5");
    ASSERT_EQ(to_string(n28), "0.00123");
    BigNumber n29 = make_big_number("-1.23e2");
    ASSERT_EQ(to_string(n29), "-123");
    BigNumber n30 = make_big_number("-1.23e-2");
    ASSERT_EQ(to_string(n30), "-0.0123");
    BigNumber n31 = make_big_number("123e36");
    ASSERT_EQ(to_string(n31), "123000000000000000000000000000000000000");

    BigNumber n50 = make_big_number("50");
    BigNumber n51 = make_big_number("-50");
    ASSERT_EQ(n50.exponent, n51.exponent);
    ASSERT_EQ(n50.is_negative, !n51.is_negative);
    ASSERT_EQ(n50.chunks, n51.chunks);
}

TEST(Constructor, Manual_LeadingZeros) {
    BigNumber n1 = make_big_number("0000123");
    ASSERT_EQ(to_string(n1), "123");
    BigNumber n2 = make_big_number("-0000123");
    ASSERT_EQ(to_string(n2), "-123");
    BigNumber n3 = make_big_number("0000.0000456");
    ASSERT_EQ(to_string(n3), "0.0000456");
    BigNumber n4 = make_big_number("-0000.0000456");
    ASSERT_EQ(to_string(n4), "-0.0000456");
}

TEST(Constructor, Manual_ZeroVariants) {
    BigNumber n1 = make_big_number("0.0");
    ASSERT_EQ(to_string(n1), "0");
    BigNumber n2 = make_big_number("-0.0");
    ASSERT_EQ(to_string(n2), "0");
    BigNumber n3 = make_big_number("0000");
    ASSERT_EQ(to_string(n3), "0");
    BigNumber n4 = make_big_number("-0000");
    ASSERT_EQ(to_string(n4), "0");
}

TEST(Constructor, Manual_OnlyDot) {
    BigNumber n1 = make_big_number(".");
    ASSERT_EQ(to_string(n1), "0");
    BigNumber n2 = make_big_number("-.0");
    ASSERT_EQ(to_string(n2), "0");
}

TEST(Constructor, Manual_ExponentEdgeCases) {
    BigNumber n1 = make_big_number("1e-100");
    ASSERT_EQ(to_string(n1), "0."
        + std::string(99, '0') + "1");
    BigNumber n2 = make_big_number("-1e-100");
    ASSERT_EQ(to_string(n2), "-0."
        + std::string(99, '0') + "1");
    BigNumber n3 = make_big_number("1e+5");
    ASSERT_EQ(to_string(n3), "100000");
    BigNumber n4 = make_big_number("-1e+5");
    ASSERT_EQ(to_string(n4), "-100000");
}

TEST(Constructor, Manual_FractionalEdgeCases) {
    BigNumber n1 = make_big_number(".0000000000000000000000000000001");
    ASSERT_EQ(to_string(n1), "0.0000000000000000000000000000001");
    BigNumber n2 = make_big_number("-.0000000000000000000000000000001");
    ASSERT_EQ(to_string(n2), "-0.0000000000000000000000000000001");
}
