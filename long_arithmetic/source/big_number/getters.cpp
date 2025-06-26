#include "getters.hpp"

#include <cstdint>

#include "big_number.hpp"
#include "constants.hpp"

namespace big_number {
    const chunks& get_mantissa( const BigNumber& number ) {
        return number.mantissa;
    }

    int32_t get_shift( const BigNumber& number ) { return number.shift; }

    BigNumberType get_type( const BigNumber& number ) { return number.type; }

    const Error& get_error( const BigNumber& number ) { return number.error; }

    bool is_negative( const BigNumber& number ) { return number.is_negative; }

    int32_t count_power( const BigNumber& number ) {
        return static_cast<int32_t>( get_size( number ) ) + get_shift( number );
    }

    chunk get_chunk( const BigNumber& number, size_t index ) {
        return number.mantissa.at( index );
    }

    chunk get_shifted_chunk( const BigNumber& number, int32_t index ) {
        int32_t chunk_index = index - get_shift( number );

        if ( chunk_index < ZERO_INT || chunk_index >= get_size( number ) )
            return ZERO_INT;

        return get_mantissa( number )[chunk_index];
    }

    size_t get_size( const BigNumber& number ) {
        return get_mantissa( number ).size();
    }

    bool is_zero( const BigNumber& number ) {
        return get_type( number ) == BigNumberType::ZERO;
    }

    bool is_nan( const BigNumber& number ) {
        return get_type( number ) == BigNumberType::NOT_A_NUMBER;
    }

    bool is_inf( const BigNumber& number ) {
        return get_type( number ) == BigNumberType::INF;
    }

    bool is_special( const BigNumber& number ) {
        return get_type( number ) != BigNumberType::DEFAULT;
    }

    bool has_same_sign( const BigNumber& lhs, const BigNumber& rhs ) {
        return is_negative( lhs ) == is_negative( rhs );
    }

    const Error& propagate_error( const BigNumber& lhs, const BigNumber& rhs ) {
        const Error& err_a = get_error( lhs );
        const Error& err_b = get_error( rhs );
        return !is_ok( err_a ) ? err_a : err_b;
    }
}
