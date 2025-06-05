#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"

const Error ROOT_FROM_NEG =
    make_error( CALCULATION_ERROR, "sqrt: negative number" );

const int ITERATIONS = 30;

BigNumber sqrt( const BigNumber& number ) {
    if ( is_lower_than( number, ZERO ) ) return make_zero( ROOT_FROM_NEG );
    if ( is_equal( number, ZERO ) || is_equal( number, ONE ) ) return number;

    BigNumber x = number;
    BigNumber last = ZERO;

    for ( int i = 0; i < ITERATIONS && !( is_equal( x, last ) ); ++i ) {
        last = x;
        x = div( add( x, div( number, x ) ), TWO );
    }
    return x;
}
