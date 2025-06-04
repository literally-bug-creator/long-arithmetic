#include <algorithm>
#include <stdexcept>
#include <vector>

#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "getters.hpp"
const BigNumber ZERO = make_big_number( "0" );
const BigNumber ONE = make_big_number( "1" );
const BigNumber TWO = make_big_number( "2" );

BigNumber sqrt(const BigNumber& number) {
    if (is_lower_than(number, ZERO))
        throw std::invalid_argument("sqrt: negative number");
    if (is_equal(number, ZERO) || is_equal(number, ONE))
        return number;
    BigNumber x = number;
    BigNumber last = ZERO;
    for (int i = 0; i < 100 && !(is_equal(x, last)); ++i) {
        last = x;
        x = div(add(x, div(number,x)), TWO); // if DIV_PRECISION is too big(5560 is a lot), sqrt might take a while
        // x = (x + number / x) / "2"_bn;
    }
    return x;
}