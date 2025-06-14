#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"

namespace big_number {
    BigNumber abs( const BigNumber& operand ) {
        return internal_make_big_number( get_chunks( operand ),
                                         get_shift( operand ),
                                         false,
                                         get_error( operand ) );
    }

    BigNumber neg( const BigNumber& operand ) {
        return internal_make_big_number( get_chunks( operand ),
                                         get_shift( operand ),
                                         !is_negative( operand ),
                                         get_error( operand ) );
    }

    bool is_equal( const BigNumber& left, const BigNumber& right ) {
        if ( is_negative( left ) != is_negative( right ) ) return false;
        if ( get_size( left ) != get_size( right ) ) return false;
        if ( get_shift( left ) != get_shift( right ) ) return false;

        for ( size_t index = 0; index < get_size( left ); index++ ) {
            int32_t left_chunk = get_chunk_direct( left, index );
            int32_t right_chunk = get_chunk_direct( right, index );

            if ( left_chunk != right_chunk ) return false;
        }

        return true;
    }

    int32_t choose_max_exp( const BigNumber& a, const BigNumber& b ) {
        int32_t a_exp = get_shift( a ) + get_size( a );
        int32_t b_exp = get_shift( b ) + get_size( b );
        return std::max( a_exp, b_exp );
    }

    int32_t choose_min_exp( const BigNumber& a, const BigNumber& b ) {
        return std::min( get_shift( a ), get_shift( b ) );
    }

    bool is_lower_than( const BigNumber& left, const BigNumber& right ) {
        if ( is_negative( left ) != is_negative( right ) )
            return is_negative( left ) > is_negative( right );

        int32_t left_max_exp = get_shift( left ) + get_size( left );
        int32_t right_max_exp = get_shift( right ) + get_size( right );

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

    Error collect_error( const BigNumber& left, const BigNumber& right ) {
        const Error& left_error = get_error( left );
        const Error& right_error = get_error( right );

        if ( !is_ok( left_error ) ) return left_error;
        if ( !is_ok( right_error ) ) return right_error;

        return get_default_error();
    }

    BigNumber compute_add( const BigNumber& augend, const BigNumber& addend ) {
        int32_t min_exp = choose_min_exp( augend, addend );
        int32_t sum_size = choose_max_exp( augend, addend ) - min_exp + 1;
        std::vector<chunk> sum_chunks( sum_size, 0 );

        chunk carry = 0;

        for ( size_t index = 0; index < sum_size; index++ ) {
            chunk sum_chunk = get_chunk( augend, index + min_exp ) +
                              get_chunk( addend, index + min_exp ) + carry;
            carry = sum_chunk / MAX_CHUNK;
            sum_chunks[index] = sum_chunk % MAX_CHUNK;
        }

        return internal_make_big_number( sum_chunks,
                                         min_exp,
                                         is_negative( augend ),
                                         collect_error( augend, addend ) );
    }

    BigNumber add( const BigNumber& augend, const BigNumber& addend ) {
        if ( is_zero( augend ) && is_zero( addend ) )
            return make_zero( get_default_error() );
        if ( is_zero( augend ) ) return addend;
        if ( is_zero( addend ) ) return augend;

        if ( is_negative( augend ) != is_negative( addend ) )
            return sub( augend, neg( addend ) );

        return compute_add( augend, addend );
    }

    BigNumber compute_sub( const BigNumber& minuend,
                           const BigNumber& subtrahend ) {
        int32_t min_exp = choose_min_exp( minuend, subtrahend );
        int32_t sub_size = choose_max_exp( minuend, subtrahend ) - min_exp + 1;
        std::vector<chunk> sub_chunks( sub_size, 0 );

        chunk borrow = 0;

        for ( size_t index = 0; index < sub_size; index++ ) {
            chunk minuend_chunk = get_chunk( minuend, index + min_exp );
            chunk subtrahend_chunk = get_chunk( subtrahend, index + min_exp );
            chunk diff = minuend_chunk - subtrahend_chunk - borrow;

            if ( minuend_chunk < ( subtrahend_chunk + borrow ) )
                diff =
                    ( MAX_CHUNK + minuend_chunk ) - subtrahend_chunk - borrow;

            borrow = ( minuend_chunk < ( subtrahend_chunk + borrow ) ); // 0 | 1
            sub_chunks[index] = diff;
        }

        return internal_make_big_number( sub_chunks,
                                         min_exp,
                                         is_negative( minuend ),
                                         collect_error( minuend, subtrahend ) );
    }

    BigNumber sub( const BigNumber& minuend, const BigNumber& subtrahend ) {
        if ( is_zero( minuend ) && is_zero( subtrahend ) )
            return make_zero( get_default_error() );
        if ( is_zero( minuend ) ) return neg( subtrahend );
        if ( is_zero( subtrahend ) ) return minuend;

        if ( is_negative( minuend ) != is_negative( subtrahend ) )
            return add( minuend, neg( subtrahend ) );

        if ( is_lower_than( abs( minuend ), abs( subtrahend ) ) )
            return neg( sub( subtrahend, minuend ) );

        return compute_sub( minuend, subtrahend );
    }

    bool is_greater_than( const BigNumber& left, const BigNumber& right ) {
        return !is_equal( left, right ) && !is_lower_than( left, right );
    }
}
