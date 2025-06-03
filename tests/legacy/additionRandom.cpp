#include "big_number.hpp"
#include "randomString.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <gmpxx.h>

constexpr int TEST_CASES = 10000;
constexpr double EPS = 1e-6;

int main() {
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
            std::cout << "[ADD FAIL] a=" << a_str << ", b=" << b_str << ", got=" << sum_bn << ", expected=" << sum_gmp << std::endl;
        }
    }
    std::cout << "Addition: " << add_ok << "/" << TEST_CASES << " passed, " << add_fail << " failed." << std::endl;
    return (add_fail == 0) ? 0 : 1;
}
