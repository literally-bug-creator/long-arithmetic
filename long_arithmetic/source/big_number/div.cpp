#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "getters.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

const size_t _DIV_PRECISION = 5560; // should be enough for most cases, but the real precision calculated differently
const BigNumber ZERO = make_big_number("0");
const BigNumber ONE = make_big_number("1");

BigNumber div(const BigNumber& x, const BigNumber& y) {
    if (is_equal(x, ZERO)) {
        return ZERO;
    }
    if (is_equal(y, ZERO)) {
        throw std::invalid_argument("Division by zero");
    }
    if (is_equal(y, ONE)) {
        return x;
    }

    if (is_equal(x, y)) {
        return ONE;
    }

    BigNumber a = x;
    // BigNumber b = y;
    // a.is_negative = false;
    // b.is_negative = false;

    int32_t a_chunks    = static_cast<int32_t>(get_size(x));
    int32_t b_chunks    = static_cast<int32_t>(get_size(y));
    int32_t a_exp       = get_exponent(x);
    int32_t b_exp       = get_exponent(y);
    int32_t a_max_exp   = a_exp + a_chunks - 1;
    int32_t b_max_exp   = b_exp + b_chunks - 1;

    // b.exponent += (a_max_exp - b_max_exp);
    BigNumber b = from_scratch(get_chunks(y), get_exponent(y) + (a_max_exp - b_max_exp), false, make_error(OK, ""));

    std::vector<chunk> result_chunks;

    int32_t integer_chunks = std::max(a_max_exp - b_max_exp + 1, 0);
    int32_t integer_digits = integer_chunks * CHUNK_DIGITS;      
    int32_t div_prec    = static_cast<int32_t>(_DIV_PRECISION);
    int32_t current_exp = a_max_exp;
    int32_t i           = 0;
    mul_chunk carry   = 0;
    mul_chunk b0      = static_cast<mul_chunk>(b.chunks.back());

    while (++i <= integer_chunks + div_prec && !is_equal(a, ZERO)) {
        carry = carry * CHUNK_BASE + get_chunk(a, current_exp);
        if (carry < b0) {
            result_chunks.push_back(0);
        } 
        else {
            chunk c = static_cast<chunk>(carry / b0);
            BigNumber c_bn = from_long_long(c);
            BigNumber prod = mul(b, c_bn);

            int attempts = 0;
            while (attempts++ < 3 && is_greater(prod, a)) {
                --c;
                prod = sub(prod, b);
            }

            if (is_greater(prod, a)) {
                chunk left = 0, right = c;
                while (left < right) {
                    chunk mid = (left + right) / 2;
                    BigNumber mid_bn = from_long_long(mid);
                    BigNumber pm = mul(b, mid_bn);
                    if (is_greater(pm, a)) {
                        right = mid;
                    } 
                    else {
                        left = mid + 1;
                    }
                }
                c = left - 1;
                prod = mul(b, from_long_long(c));
            }

            a = sub(a, prod);
            result_chunks.push_back(c);
            carry = get_chunk(a, current_exp);
        }

        --b.exponent;
        --current_exp;
    }

    int32_t result_exp = a_max_exp - b_max_exp + 1 - static_cast<int32_t>(result_chunks.size());

    std::vector<chunk> chunks_rev(result_chunks.rbegin(), result_chunks.rend());
    std::vector<chunk> result_chunks (chunks_rev.begin(), chunks_rev.end());
    BigNumber result = from_scratch(result_chunks,result_exp, x.is_negative != y.is_negative, make_error(OK, ""));
    return result;
}