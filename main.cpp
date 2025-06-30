#include <iostream>
#include "core.hpp"
#include "big_number.hpp"
int main() {
    big_number::BigNumber a = big_number::make_big_number("1");
    std::cout<<big_number::to_string(big_number::sqrt(a))<<std::endl;

    return 0;
}
