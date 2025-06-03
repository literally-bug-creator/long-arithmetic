#include <gtest/gtest.h>
#include <big_number.h>

#include <tuple>
class ArithmeticTest : public testing::TestWithParam<
                           std::tuple<std::string, std::string, std::string>> {
};
