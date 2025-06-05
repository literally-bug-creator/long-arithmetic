#include <cstdint>
#include <vector>

#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"

const int32_t DIV_PRECISION = 100; // should be enough for most cases, but the
                                   // real precision calculated differently
const BigNumber ZERO = make_big_number( "0" );
const BigNumber ONE = make_big_number( "1" );
const Error DIVISION_BY_ZERO =
    make_error( CALCULATION_ERROR, "Division by zero" );

BigNumber div( const BigNumber& x, const BigNumber& y ) {
    if ( is_equal( x, ZERO ) ) { return ZERO; }
    if ( is_equal( y, ZERO ) ) { return make_zero( DIVISION_BY_ZERO ); }
    if ( is_equal( y, ONE ) ) { return x; }
    if ( is_equal( x, y ) ) { return ONE; }

    int32_t a_max_exp = get_exponent( x ) + get_size( x ) - 1;
    int32_t b_max_exp = get_exponent( y ) + get_size( y ) - 1;

    BigNumber b = from_scratch( get_chunks( y ),
                                get_exponent( y ) + ( a_max_exp - b_max_exp ),
                                false,
                                DEFAULT_ERROR );

    int32_t current_exp = a_max_exp;
    mul_chunk b0 = get_chunks( b ).back();

    BigNumber a = abs( x );
    mul_chunk carry = 0;
    std::vector<chunk> result_chunks;
    int32_t max_chunks =
        std::max( a_max_exp - b_max_exp + 1, 0 ) + DIV_PRECISION;

    for ( int32_t i = 0; i <= max_chunks; ++i ) {
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

    int32_t result_exp = a_max_exp - b_max_exp + 1 -
                         static_cast<int32_t>( result_chunks.size() );
    std::vector<chunk> chunks_rev( result_chunks.rbegin(),
                                   result_chunks.rend() );
    std::vector<chunk> final_chunks( chunks_rev.begin(), chunks_rev.end() );
    BigNumber result = from_scratch( final_chunks,
                                     result_exp,
                                     x.is_negative != y.is_negative,
                                     make_error( OK, "" ) );
    return result;
}
