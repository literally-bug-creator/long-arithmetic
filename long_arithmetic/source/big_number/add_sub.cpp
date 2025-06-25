#include "big_number.hpp"
#include "constructors.hpp"
#include "getters.hpp"

namespace big_number {
    int32_t choose_max_exp( const BigNumber& a, const BigNumber& b ) {
        int32_t a_exp = get_shift( a ) + get_size( a );
        int32_t b_exp = get_shift( b ) + get_size( b );
        return std::max( a_exp, b_exp );
    }

    int32_t choose_min_exp( const BigNumber& a, const BigNumber& b ) {
        return std::min( get_shift( a ), get_shift( b ) );
    }

    Error collect_error( const BigNumber& left, const BigNumber& right ) {
        const Error& left_error = get_error( left );
        const Error& right_error = get_error( right );

        if ( !is_ok( left_error ) ) return left_error;
        if ( !is_ok( right_error ) ) return right_error;

        return get_default_error();
    }

    BigNumber compute_add( const BigNumber& lhs, const BigNumber& rhs ) {
        int32_t min_exp = choose_min_exp( lhs, rhs );
        int32_t sum_size = choose_max_exp( lhs, rhs ) - min_exp;
        std::vector<chunk> sum_chunks( sum_size, 0 );

        chunk carry = 0;

        for ( size_t index = 0; index < sum_size; index++ ) {
            chunk sum_chunk = get_shifted_chunk( lhs, index + min_exp ) +
                              get_shifted_chunk( rhs, index + min_exp ) + carry;
            carry = sum_chunk / MAX_CHUNK;
            sum_chunks[index] = sum_chunk % MAX_CHUNK;
        }

        if ( carry != 0 ) { sum_chunks.push_back( carry ); }

        return make_big_number( sum_chunks,
                                min_exp,
                                get_type( lhs ),
                                collect_error( lhs, rhs ),
                                is_negative( lhs ) );
    }

    BigNumber add( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( is_zero( lhs ) && is_zero( rhs ) )
            return make_zero( get_default_error() );
        if ( is_zero( lhs ) ) return rhs;
        if ( is_zero( rhs ) ) return lhs;

        if ( is_negative( lhs ) != is_negative( rhs ) )
            return sub( lhs, neg( rhs ) );

        return compute_add( lhs, rhs );
    }

    BigNumber compute_sub( const BigNumber& lhs, const BigNumber& rhs ) {
        int32_t min_exp = choose_min_exp( lhs, rhs );
        int32_t sub_size = choose_max_exp( lhs, rhs ) - min_exp;
        std::vector<chunk> sub_chunks( sub_size, 0 );

        chunk borrow = 0;

        for ( size_t index = 0; index < sub_size; index++ ) {
            chunk minuend_chunk = get_shifted_chunk( lhs, index + min_exp );
            chunk subtrahend_chunk = get_shifted_chunk( rhs, index + min_exp );
            chunk diff = minuend_chunk - subtrahend_chunk - borrow;

            if ( minuend_chunk < ( subtrahend_chunk + borrow ) )
                diff =
                    ( MAX_CHUNK + minuend_chunk ) - subtrahend_chunk - borrow;

            borrow = ( minuend_chunk < ( subtrahend_chunk + borrow ) ); // 0 | 1
            sub_chunks[index] = diff;
        }

        return make_big_number( sub_chunks,
                                min_exp,
                                get_type( lhs ),
                                collect_error( lhs, rhs ),
                                is_negative( lhs ) );
    }

    BigNumber sub( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( is_zero( lhs ) && is_zero( rhs ) )
            return make_zero( get_default_error() );
        if ( is_zero( lhs ) ) return neg( rhs );
        if ( is_zero( rhs ) ) return lhs;

        if ( is_negative( lhs ) != is_negative( rhs ) )
            return add( lhs, neg( rhs ) );

        if ( is_lower_than( abs( lhs ), abs( rhs ) ) )
            return neg( sub( rhs, lhs ) );

        return compute_sub( lhs, rhs );
    }
}
