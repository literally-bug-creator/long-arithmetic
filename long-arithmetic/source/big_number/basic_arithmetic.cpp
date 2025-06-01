#include <cstddef>
#include <cstdint>

#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.h"

BigNumber abs( const BigNumber& operand ) {
    Error error = make_error( OK, "1" ); // TODO: Change
    std::vector<chunk> chunk = get_chunks( operand );
    return from_scratch( chunk, get_exponent( operand ), false, error );
}

BigNumber neg( const BigNumber& operand ) {
    Error error = make_error( OK, "1" ); // TODO: Change
    std::vector<chunk> chunk = get_chunks( operand );
    return from_scratch(
        chunk, get_exponent( operand ), !is_negative( operand ), error );
}

bool is_equal( const BigNumber& left, const BigNumber& right ) {
    if ( is_negative( left ) != is_negative( right ) ) return false;
    if ( get_size( left ) != get_size( right ) ) return false;
    if ( get_exponent( left ) != get_exponent( right ) ) return false;

    for ( size_t index = 0; index < get_size( left ); index++ ) {
        int32_t left_chunk = get_chunk_direct( left, index );
        int32_t right_chunk = get_chunk_direct( right, index );

        if ( left_chunk != right_chunk ) return false;
    }

    return true;
}

int32_t choose_max_exp( const BigNumber& a, const BigNumber& b ) {
    int32_t a_exp = get_exponent( a ) + get_size( a );
    int32_t b_exp = get_exponent( b ) + get_size( b );
    return std::max( a_exp, b_exp );
}

int32_t choose_min_exp( const BigNumber& a, const BigNumber& b ) {
    return std::min( get_exponent( a ), get_exponent( b ) );
}

bool is_lower_than( const BigNumber& left, const BigNumber& right ) {
    if ( is_negative( left ) != is_negative( right ) )
        return is_negative( left ) > is_negative( right );

    int32_t left_max_exp = get_exponent( left ) + get_size( left );
    int32_t right_max_exp = get_exponent( right ) + get_size( right );

    if ( left_max_exp < right_max_exp ) return !is_negative( left );
    if ( left_max_exp > right_max_exp ) return is_negative( left );

    int32_t min_exp = choose_min_exp( left, right );

    for ( int32_t index = left_max_exp; index >= min_exp; index-- ) {
        chunk left_chunk = get_chunk( left, index );
        chunk right_chunk = get_chunk( right, index );

        if ( left_chunk < right_chunk ) return !is_negative( left );
        if ( left_chunk > right_chunk ) return is_negative( left );
    }

    return false;
}

BigNumber compute_add( const BigNumber& augend, const BigNumber& addend ) {
    int32_t min_exp = choose_min_exp( augend, addend );
    int32_t sum_size = choose_max_exp( augend, addend ) - min_exp + 1;
    std::vector<chunk> sum_chunks( sum_size, 0 );

    chunk carry = 0;

    for ( size_t index = 0; index < sum_size; index++ ) {
        chunk sum_chunk = get_chunk( augend, index + min_exp ) +
                          get_chunk( addend, index + min_exp ) + carry;
        carry = sum_chunk / CHUNK_BASE;
        sum_chunks[index] = sum_chunk % CHUNK_BASE;
    }
    // strip zeros)))

    Error error = make_error( OK, "" ); // TODO: Change
    return from_scratch( sum_chunks, min_exp, is_negative( augend ), error );
}

BigNumber add( const BigNumber& augend, const BigNumber& addend ) {
    if ( is_negative( augend ) != is_negative( addend ) )
        return sub( augend, neg( addend ) );

    if ( is_zero( augend ) ) return addend;
    if ( is_zero( addend ) ) return augend;

    return compute_add( augend, addend );
}

BigNumber compute_sub( const BigNumber& minuend, const BigNumber& subtrahend ) {
    int32_t min_exp = choose_min_exp( minuend, subtrahend );
    int32_t sub_size = choose_max_exp( minuend, subtrahend ) - min_exp + 1;
    std::vector<chunk> sub_chunks( sub_size, 0 );

    chunk borrow = 0;

    for ( size_t index = 0; index < sub_size; index++ ) {
        chunk minuend_chunk = get_chunk( minuend, index + min_exp );
        chunk subtrahend_chunk = get_chunk( subtrahend, index + min_exp );
        chunk diff = minuend_chunk - subtrahend_chunk - borrow;

        if ( minuend_chunk < ( subtrahend_chunk + borrow ) )
            diff = ( CHUNK_BASE + minuend_chunk ) - subtrahend_chunk - borrow;

        borrow = ( minuend_chunk < ( subtrahend_chunk + borrow ) ); // 0 | 1
        sub_chunks[index] = diff;
    }
    // strip zeros)))
    Error error = make_error( OK, "" ); // TODO: Change
    return from_scratch( sub_chunks, min_exp, is_negative( minuend ), error );
}

BigNumber sub( const BigNumber& minuend, const BigNumber& subtrahend ) {
    if ( is_negative( minuend ) != is_negative( subtrahend ) )
        return add( minuend, neg( subtrahend ) );

    if ( is_zero( minuend ) ) return neg( subtrahend );
    if ( is_zero( subtrahend ) ) return minuend;

    if ( is_lower_than( abs( minuend ), abs( subtrahend ) ) )
        return neg( sub( subtrahend, minuend ) );

    return compute_sub( minuend, subtrahend );
}
