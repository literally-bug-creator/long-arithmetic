#include <cstdint>
#include <vector>

#include "big_number.hpp"
#include "constructors.hpp"
#include "getters.hpp"
#include "basic_arithmetic.hpp"

namespace big_number {
    const size_t TO_RESERVE = 5560;
    const Error DIVISION_BY_ZERO =
        make_error( CALCULATION_ERROR, "Division by zero" );

    int32_t compute_max_exp( const BigNumber& number ) {
        return get_exponent( number ) + get_size( number ) - 1;
    }

    BigNumber div_old( const BigNumber& x, const BigNumber& y, int32_t precision ) {
        if ( is_equal( x, ZERO ) ) { return ZERO; }
        if ( is_equal( y, ZERO ) ) { return make_zero( DIVISION_BY_ZERO ); }
        if ( is_equal( y, ONE ) ) { return x; }
        if ( is_equal( x, y ) ) { return ONE; }
        // if (get_size(x) + get_size(y)>=DIV_PRECISION) precision = 1;
        BigNumber b = from_scratch(
            get_chunks( y ),
            get_exponent( y ) + ( compute_max_exp( x ) - compute_max_exp( y ) ),
            false,
            DEFAULT_ERROR );

        BigNumber a = abs( x );
        mul_chunk carry = 0;
        std::vector<chunk> result_chunks;
        result_chunks.reserve( TO_RESERVE );
        int32_t max_chunks =
            std::max( compute_max_exp( x ) - compute_max_exp( y ) + 1, 0 ) +
            precision;
        int32_t current_exp = compute_max_exp( x );
        mul_chunk b0 = get_chunks( b ).back();

        for ( int32_t i = 0; i <= max_chunks; ++i ) {
            // if (result_chunks.size()>=5556) break;
            if ( is_equal( a, ZERO ) ) break;

            carry = carry * CHUNK_BASE + get_chunk( a, current_exp );

            if ( carry < b0 )
                result_chunks.push_back( 0 );

            else {
                chunk c = carry / b0;
                BigNumber prod = mul( b, from_long_long( c ) );

                int attempts = 0;
                while ( attempts++ < 3 &&
                        is_greater_than( prod, a ) ) { // TODO: 3 to constant
                    --c;
                    prod = sub( prod, b );
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
                }

                a = sub( a, prod );
                result_chunks.push_back( c );
                carry = get_chunk( a, current_exp );
            }

            --b.exponent;
            --current_exp;
        }

        int32_t result_exp = compute_max_exp( x ) - compute_max_exp( y ) + 1 -
                             result_chunks.size();
        std::vector<chunk> chunks_rev( result_chunks.rbegin(),
                                       result_chunks.rend() );
        std::vector<chunk> final_chunks( chunks_rev.begin(), chunks_rev.end() );
        bool sign = is_negative( x ) != is_negative( y );
        return from_scratch( final_chunks, result_exp, sign, DEFAULT_ERROR );
    }
    BigNumber div(const BigNumber& x, const BigNumber& y, int32_t precision) {
        if (is_equal(x, ZERO)) return ZERO;
        if (is_equal(y, ZERO)) return make_zero(DIVISION_BY_ZERO);
        if (is_equal(y, ONE)) return x;
        if (is_equal(x, y)) return ONE;

        int32_t exp_x = get_exponent(x) + get_size(x);
        int32_t exp_y = get_exponent(y) + get_size(y);
        int32_t scale = std::max(0, exp_y - exp_x);
        BigNumber scaled_x = shift(x, scale);

        BigNumber one = from_long_long(1);
        BigNumber invY = div_old(one, y, precision + 10);

        BigNumber quotient = mul(scaled_x, invY);

        quotient.exponent -= scale;
        
        // Remove trailing zeros
        while (!quotient.chunks.empty() && quotient.chunks.back() == 0) {
            quotient.chunks.pop_back();
            quotient.exponent++;
        }
        
        return quotient;
    }

}
