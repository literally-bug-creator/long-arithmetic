#include <gtest/gtest.h>
#include "big_number.hpp"
#include <string>

// TEST(DivisionManual, DivideByOne) {
//     BigNumber a = make_big_number("123456789");
//     BigNumber b = make_big_number("1");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "123456789");
// }

// TEST(DivisionManual, DivideSelf) {
//     BigNumber a = make_big_number("987654321");
//     BigNumber result = div(a, a);
//     EXPECT_EQ(to_string(result), "1");
// }

// TEST(DivisionManual, DivideZero) {
//     BigNumber a = make_big_number("0");
//     BigNumber b = make_big_number("12345");
//     BigNumber result = div(a, b);
//     EXPECT_EQ(to_string(result), "0");
// }

// TEST(DivisionManual, SimpleDivision) {
//     BigNumber a = make_big_number("100");
//     BigNumber b = make_big_number("3");
//     BigNumber result = div(a, b);
//     // Ожидаемое значение: 33.333... (первые 2 знака после запятой)
//     std::string res_str = to_string(result);
//     EXPECT_TRUE(res_str.substr(0, 2) == "33");
// }

TEST(DivisionManual, NegativeDivision) {
    BigNumber a = make_big_number("-50");
    BigNumber b = make_big_number("2");
    BigNumber result = div(a, b);
    EXPECT_EQ(to_string(result), "-25");
}
TEST(DivisionManual, x1) {
    BigNumber a = make_big_number("33");
    BigNumber b = make_big_number("3");
    BigNumber result = div(a, b);
    EXPECT_EQ(to_string(result), "11");
}
TEST(DivisionManual, x2) {
    BigNumber a = make_big_number("20");
    BigNumber b = make_big_number("2");
    BigNumber result = div(a, b);
    EXPECT_EQ(to_string(result), "10");
}
TEST(DivisionManual, x3) {
    BigNumber a = make_big_number("5");
    BigNumber b = make_big_number("2");
    BigNumber result = div(a, b);
    EXPECT_EQ(to_string(result), "2.5");
}

TEST(DivisionManual, x4) {
    BigNumber a = make_big_number("50");
    BigNumber b = make_big_number("2");
    BigNumber result = div(a, b);
    EXPECT_EQ(to_string(result), "25");
}

