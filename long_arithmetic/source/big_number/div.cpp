#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "getters.hpp"

const size_t _DIV_PRECISION = 20; // should be enough for most cases, but the
                                    // real precision calculated differently
const BigNumber ZERO = make_big_number( "0" );
const BigNumber ONE = make_big_number( "1" );

BigNumber div( const BigNumber& x, const BigNumber& y ) {
    // std::cout << "div: x=" << to_string(x) << ", y=" << to_string(y) << std::endl;
    if ( is_equal( x, ZERO ) ) { /*std::cout << "x is ZERO" << std::endl;*/ return ZERO; }
    if ( is_equal( y, ZERO ) ) {
        // std::cout << "y is ZERO (division by zero)" << std::endl;
        throw std::invalid_argument( "Division by zero" );
    }
    if ( is_equal( y, ONE ) ) { /*std::cout << "y is ONE" << std::endl;*/ return x; }
    if ( is_equal( x, y ) ) { /*std::cout << "x == y" << std::endl;*/ return ONE; }

    BigNumber a = x;
    // BigNumber b = y;
    a.is_negative = false;
    // b.is_negative = false;
    int32_t a_chunks = static_cast<int32_t>( get_size( x ) );
    int32_t b_chunks = static_cast<int32_t>( get_size( y ) );
    int32_t a_exp = get_exponent( x );
    int32_t b_exp = get_exponent( y );
    int32_t a_max_exp = a_exp + a_chunks - 1;
    int32_t b_max_exp = b_exp + b_chunks - 1;

    // std::cout << "a_chunks=" << a_chunks << ", b_chunks=" << b_chunks << std::endl;
    // std::cout << "a_exp=" << a_exp << ", b_exp=" << b_exp << std::endl;
    // std::cout << "a_max_exp=" << a_max_exp << ", b_max_exp=" << b_max_exp << std::endl;

    BigNumber b = from_scratch( get_chunks( y ),
                                get_exponent( y ) + ( a_max_exp - b_max_exp ),
                                false,
                                make_error( OK, "" ) );
    // std::cout << "b (shifted)=" << to_string(b) << ", b.exp=" << b.exponent << std::endl;

    std::vector<chunk> result_chunks;

    int32_t integer_chunks = std::max( a_max_exp - b_max_exp + 1, 0 );
    int32_t integer_digits = integer_chunks * CHUNK_DIGITS;
    int32_t div_prec = static_cast<int32_t>( _DIV_PRECISION );
    int32_t current_exp = a_max_exp;
    int32_t i = 0;
    mul_chunk carry = 0;
    mul_chunk b0 = static_cast<mul_chunk>( b.chunks.back() );

    // std::cout << "integer_chunks=" << integer_chunks << ", div_prec=" << div_prec << std::endl;

    while ( ++i <= integer_chunks + div_prec && !is_equal( a, ZERO ) ) {
        carry = carry * CHUNK_BASE + get_chunk( a, current_exp );
        // std::cout << "[loop] i=" << i << ", carry=" << static_cast<unsigned long long>(carry) << ", current_exp=" << current_exp << std::endl;
        if ( carry < b0 ) {
            result_chunks.push_back( 0 );
            // std::cout << "  carry < b0, push 0" << std::endl;
        } else {
            chunk c = static_cast<chunk>( carry / b0 );
            // std::cout << "  c=" << c << std::endl;
            BigNumber c_bn = from_long_long( c );
            BigNumber prod = mul( b, c_bn );
            // std::cout << "  prod=" << to_string(prod) << std::endl;

            int attempts = 0;
            while ( attempts++ < 3 && is_greater_than( prod, a ) ) {
                --c;
                prod = sub( prod, b );
                // std::cout << "    prod > a, decrease c to " << c << ", prod=" << to_string(prod) << std::endl;
            }

            if ( is_greater_than( prod, a ) ) {
                chunk left = 0, right = c;
                while ( left < right ) {
                    chunk mid = left + ( right - left ) / 2;
                    BigNumber mid_bn = from_long_long( mid );
                    BigNumber pm = mul( b, mid_bn );
                    if ( is_greater_than( pm, a ) ) {
                        right = mid;
                    } else {
                        left = mid + 1;
                    }
                }
                c = left - 1;
                prod = mul( b, from_long_long( c ) );
                // std::cout << "    binary search: c=" << c << ", prod=" << to_string(prod) << std::endl;
            }

            a = sub( a, prod );
            // std::cout << "  a after sub = " << to_string(a) << std::endl;
            result_chunks.push_back( c );
            carry = get_chunk( a, current_exp );
        }

        --b.exponent;
        --current_exp;
    }

    int32_t result_exp = a_max_exp - b_max_exp + 1 -
                         static_cast<int32_t>( result_chunks.size() );
    std::vector<chunk> chunks_rev( result_chunks.rbegin(),
                                   result_chunks.rend() );
    std::vector<chunk> final_chunks( chunks_rev.begin(), chunks_rev.end() );
    // std::cout << "final_chunks: [";
    // for (size_t idx = 0; idx < final_chunks.size(); ++idx) {
    //     std::cout << static_cast<unsigned long long>(final_chunks[idx]);
    //     if (idx + 1 != final_chunks.size()) std::cout << ", ";
    // }
    // std::cout << "]\n";
    // std::cout << "result_exp=" << result_exp << std::endl;
    BigNumber result = from_scratch( final_chunks,
                                     result_exp,
                                     x.is_negative != y.is_negative,
                                     make_error( OK, "" ) );
    // std::cout << "result=" << to_string(result) << ", result.exp=" << result.exponent << ", result.is_negative=" << result.is_negative << std::endl;
    return result;
}
