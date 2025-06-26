#include <algorithm>
#include <utility>

#include "big_number.hpp"
#include "constants.hpp"
#include "constructors.hpp"
#include "error.hpp"
#include "getters.hpp"

namespace big_number {
    using range = std::pair<int32_t, size_t>;

    inline const Error& propagate_error( const BigNumber& a,
                                         const BigNumber& b ) {
        const Error& err_a = get_error( a );
        const Error& err_b = get_error( b );
        return !is_ok( err_a ) ? err_a : err_b;
    }

    inline bool has_same_sign( const BigNumber& a, const BigNumber& b ) {
        return is_negative( a ) == is_negative( b );
    }

    inline range calculate_range( const BigNumber& a, const BigNumber& b ) {
        int32_t min_exp = std::min( get_shift( a ), get_shift( b ) );
        int32_t max_exp =
            std::max( get_shift( a ) + static_cast<int32_t>( get_size( a ) ),
                      get_shift( b ) + static_cast<int32_t>( get_size( b ) ) );
        return { min_exp, static_cast<size_t>( max_exp - min_exp ) };
    }

    BigNumber perform_addition( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( is_zero( lhs ) ) return rhs;
        if ( is_zero( rhs ) ) return lhs;

        auto [min_exp, range_size] = calculate_range( lhs, rhs );

        chunks result_chunks( range_size, 0 );
        result_chunks.reserve( range_size + 1 );

        chunk carry = 0;
        auto result_it = result_chunks.begin();
        int32_t pos = min_exp;

        for ( const auto result_end = result_chunks.end();
              result_it != result_end;
              ++result_it, ++pos ) {
            const chunk lhs_chunk = get_shifted_chunk( lhs, pos );
            const chunk rhs_chunk = get_shifted_chunk( rhs, pos );
            const chunk sum = lhs_chunk + rhs_chunk + carry;

            if ( sum < MAX_CHUNK ) {
                *result_it = sum;
                carry = 0;
            } else {
                *result_it = sum - MAX_CHUNK;
                carry = 1;
            }
        }

        if ( carry != 0 ) { result_chunks.push_back( carry ); }

        return make_big_number( std::move( result_chunks ),
                                min_exp,
                                BigNumberType::DEFAULT,
                                propagate_error( lhs, rhs ),
                                is_negative( lhs ) );
    }

    BigNumber perform_subtraction( const BigNumber& lhs,
                                   const BigNumber& rhs ) {
        if ( is_zero( rhs ) ) return lhs;
        if ( is_zero( lhs ) ) return neg( rhs );

        auto [min_exp, range_size] = calculate_range( lhs, rhs );

        chunks result_chunks( range_size, 0 );

        chunk borrow = 0;
        auto result_it = result_chunks.begin();
        int32_t pos = min_exp;

        for ( const auto result_end = result_chunks.end();
              result_it != result_end;
              ++result_it, ++pos ) {
            const chunk minuend = get_shifted_chunk( lhs, pos );
            const chunk subtrahend_with_borrow =
                get_shifted_chunk( rhs, pos ) + borrow;

            if ( minuend >= subtrahend_with_borrow ) {
                *result_it = minuend - subtrahend_with_borrow;
                borrow = 0;
            } else {
                *result_it = MAX_CHUNK + minuend - subtrahend_with_borrow;
                borrow = 1;
            }
        }

        return make_big_number( std::move( result_chunks ),
                                min_exp,
                                BigNumberType::DEFAULT,
                                propagate_error( lhs, rhs ),
                                is_negative( lhs ) );
    }

    BigNumber handle_special_addition( const BigNumber& lhs,
                                       const BigNumber& rhs ) {
        const Error& error = propagate_error( lhs, rhs );

        switch ( get_type( lhs ) ) {
        case BigNumberType::DEFAULT:
            switch ( get_type( rhs ) ) {
            case BigNumberType::DEFAULT:
                return perform_addition( lhs, rhs );
            case BigNumberType::ZERO:
                return lhs;
            case BigNumberType::INF:
                return make_inf( error, is_negative( rhs ) );
            case BigNumberType::NOT_A_NUMBER:
                return make_nan( error );
            }
            break;

        case BigNumberType::ZERO:
            return rhs;

        case BigNumberType::INF:
            switch ( get_type( rhs ) ) {
            case BigNumberType::DEFAULT:
            case BigNumberType::ZERO:
                return make_inf( error, is_negative( lhs ) );
            case BigNumberType::INF:
                return has_same_sign( lhs, rhs )
                           ? make_inf( error, is_negative( lhs ) )
                           : make_nan( error );
            case BigNumberType::NOT_A_NUMBER:
                return make_nan( error );
            }
            break;

        case BigNumberType::NOT_A_NUMBER:
            return make_nan( error );
        }

        return make_nan( error );
    }

    BigNumber handle_special_subtraction( const BigNumber& lhs,
                                          const BigNumber& rhs ) {
        const Error& error = propagate_error( lhs, rhs );

        switch ( get_type( lhs ) ) {
        case BigNumberType::DEFAULT:
            switch ( get_type( rhs ) ) {
            case BigNumberType::DEFAULT:
                return perform_subtraction( lhs, rhs );
            case BigNumberType::ZERO:
                return lhs;
            case BigNumberType::INF:
                return make_inf( error, !is_negative( rhs ) );
            case BigNumberType::NOT_A_NUMBER:
                return make_nan( error );
            }
            break;

        case BigNumberType::ZERO:
            switch ( get_type( rhs ) ) {
            case BigNumberType::DEFAULT:
                return neg( rhs );
            case BigNumberType::ZERO:
                return make_zero( error );
            case BigNumberType::INF:
                return make_inf( error, !is_negative( rhs ) );
            case BigNumberType::NOT_A_NUMBER:
                return make_nan( error );
            }
            break;

        case BigNumberType::INF:
            switch ( get_type( rhs ) ) {
            case BigNumberType::DEFAULT:
            case BigNumberType::ZERO:
                return make_inf( error, is_negative( lhs ) );
            case BigNumberType::INF:
                return !has_same_sign( lhs, rhs )
                           ? make_inf( error, is_negative( lhs ) )
                           : make_nan( error );
            case BigNumberType::NOT_A_NUMBER:
                return make_nan( error );
            }
            break;

        case BigNumberType::NOT_A_NUMBER:
            return make_nan( error );
        }

        return make_nan( error );
    }

    BigNumber add( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( &lhs == &rhs ) return perform_addition( lhs, lhs );
        if ( is_special( lhs ) || is_special( rhs ) )
            return handle_special_addition( lhs, rhs );

        if ( !has_same_sign( lhs, rhs ) ) return sub( lhs, neg( rhs ) );
        return perform_addition( lhs, rhs );
    }

    BigNumber sub( const BigNumber& lhs,
                   const BigNumber& rhs ) { // TODO: Refactor
        if ( &lhs == &rhs ) return make_zero( propagate_error( lhs, rhs ) );
        if ( is_special( lhs ) || is_special( rhs ) )
            return handle_special_subtraction( lhs, rhs );

        if ( !has_same_sign( lhs, rhs ) ) return add( lhs, neg( rhs ) );

        if ( is_lower_than( abs( lhs ), abs( rhs ) ) )
            return neg( sub( rhs, lhs ) );

        return perform_subtraction( lhs, rhs );
    }
}
