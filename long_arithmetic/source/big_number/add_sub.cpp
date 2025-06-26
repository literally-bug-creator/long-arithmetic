#include <algorithm>
#include <utility>

#include "big_number.hpp"
#include "constants.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"

namespace big_number {
    using range = std::pair<int32_t, size_t>;

    range calculate_range( const BigNumber& a, const BigNumber& b ) {
        int32_t min_exp = std::min( get_shift( a ), get_shift( b ) );
        int32_t max_exp =
            std::max( get_shift( a ) + static_cast<int32_t>( get_size( a ) ),
                      get_shift( b ) + static_cast<int32_t>( get_size( b ) ) );
        return { min_exp, static_cast<size_t>( max_exp - min_exp ) };
    }

    BigNumber perform_addition( const BigNumber& lhs, const BigNumber& rhs ) {
        auto [min_exp, range_size] = calculate_range( lhs, rhs );

        chunks result_chunks( range_size, ZERO_INT );
        result_chunks.reserve( range_size + ONE_INT );

        chunk carry = 0;

        for ( size_t i = 0; i < range_size; ++i ) {
            const int32_t pos = min_exp + static_cast<int32_t>( i );
            const chunk lhs_chunk = get_shifted_chunk( lhs, pos );
            const chunk rhs_chunk = get_shifted_chunk( rhs, pos );
            const chunk sum = lhs_chunk + rhs_chunk + carry;

            if ( sum < MAX_CHUNK ) {
                result_chunks[i] = sum;
                carry = ZERO_INT;
            } else {
                result_chunks[i] = sum - MAX_CHUNK;
                carry = ONE_INT;
            }
        }

        if ( carry != ZERO_INT ) { result_chunks.push_back( carry ); }

        return make_big_number( std::move( result_chunks ),
                                min_exp,
                                BigNumberType::DEFAULT,
                                propagate_error( lhs, rhs ),
                                is_negative( lhs ) );
    }

    BigNumber perform_subtraction( const BigNumber& lhs,
                                   const BigNumber& rhs ) {
        auto [min_exp, range_size] = calculate_range( lhs, rhs );
        chunks result_chunks( range_size, 0 );
        chunk borrow = 0;

        for ( size_t i = 0; i < range_size; ++i ) {
            const int32_t pos = min_exp + static_cast<int32_t>( i );
            const chunk minuend = get_shifted_chunk( lhs, pos );
            const chunk subtrahend_with_borrow =
                get_shifted_chunk( rhs, pos ) + borrow;

            if ( minuend >= subtrahend_with_borrow ) {
                result_chunks[i] = minuend - subtrahend_with_borrow;
                borrow = 0;
            } else {
                result_chunks[i] = MAX_CHUNK + minuend - subtrahend_with_borrow;
                borrow = 1;
            }
        }

        return make_big_number( std::move( result_chunks ),
                                min_exp,
                                BigNumberType::DEFAULT,
                                propagate_error( lhs, rhs ),
                                is_negative( lhs ) );
    }

    BigNumber handle_add_to_inf( const BigNumber& lhs,
                                 const BigNumber& rhs,
                                 const Error& error ) {
        switch ( get_type( rhs ) ) {
        case BigNumberType::DEFAULT:
            return lhs;
        case BigNumberType::ZERO:
            return lhs;
        case BigNumberType::NOT_A_NUMBER:
            return make_nan( error );
        case BigNumberType::INF:
            return has_same_sign( lhs, rhs )
                       ? make_inf( error, is_negative( lhs ) )
                       : make_nan( error );
        }
    }

    BigNumber handle_special_addition( const BigNumber& lhs,
                                       const BigNumber& rhs ) {
        if ( !is_special( lhs ) && is_special( rhs ) )
            return handle_special_addition( rhs, lhs );

        const Error& error = propagate_error( lhs, rhs );

        switch ( get_type( lhs ) ) {
        case BigNumberType::ZERO:
            return rhs;
        case BigNumberType::INF:
            return handle_add_to_inf( lhs, rhs, error );
        case BigNumberType::NOT_A_NUMBER:
            return make_nan( error );
        case BigNumberType::DEFAULT:
            return perform_addition( lhs, rhs );
        }
    }

    BigNumber handle_sub_from_inf( const BigNumber& lhs,
                                   const BigNumber& rhs,
                                   const Error& error ) {
        switch ( get_type( rhs ) ) {
        case BigNumberType::DEFAULT:
            return make_inf( error, is_negative( lhs ) );
        case BigNumberType::ZERO:
            return make_inf( error, is_negative( lhs ) );
        case BigNumberType::NOT_A_NUMBER:
            return make_nan( error );
        case BigNumberType::INF:
            return !has_same_sign( lhs, rhs )
                       ? make_inf( error, is_negative( lhs ) )
                       : make_nan( error );
        }
    }

    BigNumber handle_sub_from_zero( const BigNumber& lhs,
                                    const BigNumber& rhs,
                                    const Error& error ) {
        switch ( get_type( rhs ) ) {
        case BigNumberType::ZERO:
            return make_zero( error );
        case BigNumberType::NOT_A_NUMBER:
            return make_nan( error );
        case BigNumberType::INF:
            return make_inf( error, !is_negative( rhs ) );
        case BigNumberType::DEFAULT:
            return neg( rhs );
        }
    }

    BigNumber handle_special_subtraction( const BigNumber& lhs,
                                          const BigNumber& rhs ) {
        if ( !is_special( lhs ) && is_special( rhs ) )
            return neg( handle_special_subtraction( rhs, lhs ) );

        const Error& error = propagate_error( lhs, rhs );

        switch ( get_type( lhs ) ) {
        case BigNumberType::ZERO:
            return handle_sub_from_zero( lhs, rhs, error );
        case BigNumberType::INF:
            return handle_sub_from_inf( lhs, rhs, error );
        case BigNumberType::NOT_A_NUMBER:
            return make_nan( error );
        case BigNumberType::DEFAULT:
            return perform_subtraction( lhs, rhs );
        }
    }

    BigNumber add( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( &lhs == &rhs ) return perform_addition( lhs, lhs );
        if ( is_special( lhs ) || is_special( rhs ) )
            return handle_special_addition( lhs, rhs );

        if ( !has_same_sign( lhs, rhs ) ) return sub( lhs, neg( rhs ) );
        return perform_addition( lhs, rhs );
    }

    BigNumber sub( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( &lhs == &rhs ) return make_zero( propagate_error( lhs, rhs ) );

        if ( is_special( lhs ) || is_special( rhs ) )
            return handle_special_subtraction( lhs, rhs );

        if ( !has_same_sign( lhs, rhs ) ) return add( lhs, neg( rhs ) );

        if ( is_lower_than( abs( lhs ), abs( rhs ) ) )
            return neg( sub( rhs, lhs ) );

        return perform_subtraction( lhs, rhs );
    }
}
