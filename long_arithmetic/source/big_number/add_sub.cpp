#include "big_number.hpp"
#include "constructors.hpp"
#include "error.hpp"
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

    const Error& collect_error( const BigNumber& left,
                                const BigNumber& right ) {
        const Error& lhs = get_error( left );
        const Error& rhs = get_error( right );
        return !is_ok( lhs ) ? lhs : rhs;
    }

    BigNumber compute_add( const BigNumber& lhs, const BigNumber& rhs ) {
        int32_t min_exp = choose_min_exp( lhs, rhs );
        int32_t sum_size = choose_max_exp( lhs, rhs ) - min_exp;
        chunks sum_chunks( sum_size, 0 );

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

    BigNumber add_to_inf( const BigNumber& lhs, const BigNumber& rhs ) {
        switch ( get_type( rhs ) ) {
        case BigNumberType::DEFAULT:
            return lhs;

        case BigNumberType::ZERO:
            return lhs;

        case BigNumberType::NOT_A_NUMBER:
            return make_nan( collect_error( lhs, rhs ) );

        case BigNumberType::INF:
            bool has_same_sign = is_negative( lhs ) == is_negative( rhs );
            return has_same_sign ? lhs : make_nan( collect_error( lhs, rhs ) );
        }
    }

    BigNumber add_special( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( !is_special( lhs ) && is_special( rhs ) ) {
            return add_special( rhs, lhs );
        }

        switch ( get_type( lhs ) ) {
        case BigNumberType::ZERO:
            return rhs;

        case BigNumberType::INF:
            return add_to_inf( lhs, rhs );

        case BigNumberType::NOT_A_NUMBER:
            return make_nan( collect_error( lhs, rhs ) );

        case BigNumberType::DEFAULT:
            return compute_add( lhs, rhs );
        }
    }

    BigNumber add( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( is_special( lhs ) || is_special( rhs ) )
            return add_special( lhs, rhs );

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

            borrow = ( minuend_chunk < ( subtrahend_chunk + borrow ) );
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
