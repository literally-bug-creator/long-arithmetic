#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"
#include <iostream>
#include <cmath>
#include <functional>
#include <algorithm>

namespace big_number {
    
    static int32_t compute_max_exp( const BigNumber& number ) {
        return get_exponent( number ) + get_size( number ) - 1;
    }
    const Error ROOT_FROM_NEG =
    make_error( CALCULATION_ERROR, "sqrt: negative number" );
        const int ITERATIONS = 35;
        const size_t MAX_CHUNKS = 5556; 

BigNumber sqrt(const BigNumber& number) {
    static const BigNumber ZERO = from_long_long(0);
    static const BigNumber ONE = from_long_long(1);
    static const BigNumber HALF = make_big_number("0.5");
    static int PRECISION = std::max((size_t) 1, (size_t)DIV_PRECISION - get_size(number));
    const size_t MAX_CHUNKS = 5556;

    if (is_lower_than(number, ZERO)) 
        return make_zero(ROOT_FROM_NEG);
    if (is_equal(number, ZERO) || is_equal(number, ONE))
        return number;

    BigNumber x = number;
    BigNumber last = ZERO;

    for (int i = 0; i < ITERATIONS; ++i) {
        if (is_equal(x, last)) break;
        last = x;

        BigNumber inv_x = div_old(ONE, x, PRECISION);

        size_t inv_size = get_size(inv_x);
        if (inv_size > MAX_CHUNKS) {
            size_t excess = inv_size - MAX_CHUNKS;
            std::vector<chunk> new_inv_chunks(
                inv_x.chunks.begin() + excess,
                inv_x.chunks.end()
            );
            inv_x = from_scratch(
                new_inv_chunks,
                get_exponent(inv_x),
                is_negative(inv_x),
                get_error(inv_x)
            );
        }

        BigNumber term = mul(number, inv_x);
        BigNumber sum = add(x, term);
        x = mul(sum, HALF);

        size_t x_size = get_size(x);
        if (x_size > MAX_CHUNKS) {
            size_t excess = x_size - MAX_CHUNKS;
            std::vector<chunk> new_x_chunks(
                x.chunks.begin() + excess,
                x.chunks.end()
            );
            x = from_scratch(
                new_x_chunks,
                get_exponent(x),
                is_negative(x),
                get_error(x)
            );
        }
    }
    return x;
}
    
}
