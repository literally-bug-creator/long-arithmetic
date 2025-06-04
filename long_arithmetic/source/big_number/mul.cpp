#include "basic_arithmetic.hpp"
#include "big_number.hpp"
#include "constructors.hpp"
#include "getters.hpp"
#include <iostream>

#define MULTIPLY_THRESHOLD 42

BigNumber simple_mul( const BigNumber& multiplicand,
                      const BigNumber& multiplier ) {
    if ( is_zero( multiplicand ) || is_zero( multiplier ) ) return make_zero();
    std::vector<chunk> chunks(
        get_size( multiplicand ) + get_size( multiplier ), 0 );
    chunk carry = 0;
    for ( int32_t i = 0; i < get_size( multiplicand ); ++i ) {
        carry = 0;
        for ( int32_t j = 0; j < get_size( multiplier ); ++j ) {
            mul_chunk l_chunk = (mul_chunk)get_chunk_direct( multiplicand, i );
            mul_chunk r_chunk = (mul_chunk)get_chunk_direct( multiplier, j );
            mul_chunk product = l_chunk * r_chunk + chunks[i + j] + carry;
            carry = product / CHUNK_BASE;
            chunks[i + j] = product % CHUNK_BASE;
        }
        chunks[i + get_size( multiplier )] = carry;
    }
    return from_scratch(
        chunks,
        get_exponent( multiplicand ) + get_exponent( multiplier ),
        is_negative( multiplicand ) != is_negative( multiplier ),
        get_error( multiplicand ) );
}

BigNumber mul( const BigNumber& multiplicand, const BigNumber& multiplier ) {
    if ( is_zero( multiplicand ) || is_zero( multiplier ) ) return make_zero();
    if ( get_size( multiplicand ) <= MULTIPLY_THRESHOLD ||
         get_size( multiplier ) <= MULTIPLY_THRESHOLD )
        return simple_mul( multiplicand, multiplier );
    size_t m = std::max( get_size( multiplicand ), get_size( multiplier ) ) / 2;
    BigNumber a_0, a_1, b_0, b_1;
    if ( m >= get_size( multiplicand ) ) {
        a_0 = make_zero();
        a_1 = from_scratch( get_chunks( multiplicand ),
                            get_exponent( multiplicand ),
                            is_negative( multiplicand ),
                            get_error( multiplicand ) );
    } else {
        a_0 = from_iterator( get_chunks( multiplicand ).begin() + m,
                             get_chunks( multiplicand ).end() );
        a_1 = from_iterator( get_chunks( multiplicand ).begin(),
                             get_chunks( multiplicand ).begin() + m );
    }
    if ( m >= get_size( multiplier ) ) {
        b_0 = make_zero();
        b_1 = from_scratch( get_chunks( multiplier ),
                            get_exponent( multiplier ),
                            is_negative( multiplier ),
                            get_error( multiplier ) );
    } else {
        b_0 = from_iterator( get_chunks( multiplier ).begin() + m,
                             get_chunks( multiplier ).end() );
        b_1 = from_iterator( get_chunks( multiplier ).begin(),
                             get_chunks( multiplier ).begin() + m );
    }
    BigNumber z0 = mul( a_0, b_0 );
    BigNumber z1 = mul( add( a_0, a_1 ), add( b_0, b_1 ) );
    BigNumber z2 = mul( a_1, b_1 );
    BigNumber z0_shifted = shift( z0, 2 * m );
    BigNumber z1_shifted = shift( sub( sub( z1, z0 ), z2 ), m );
    BigNumber result = add( add( z0_shifted, z1_shifted ), z2 );
    bool is_neg = is_negative( multiplicand ) != is_negative( multiplier );
    int32_t exp = get_exponent( multiplicand ) + get_exponent( multiplier );
    return from_scratch(
        get_chunks( result ), exp, is_neg, get_error( result ) );
}
