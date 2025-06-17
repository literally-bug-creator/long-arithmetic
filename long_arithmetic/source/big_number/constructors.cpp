#include "constructors.hpp"

#include <cstdlib>

#include "big_number.hpp"
#include "constants.hpp"

namespace big_number {
    BigNumber make_zero( const Error& error ) noexcept {
        return { {}, ZERO_INT, BigNumberType::ZERO, error, false };
    }

    BigNumber make_nan( const Error& error ) noexcept {
        return { {}, ZERO_INT, BigNumberType::NAN, error, false };
    }

    BigNumber make_inf( const Error& error, bool is_negative ) noexcept {
        return { {}, ZERO_INT, BigNumberType::INF, error, is_negative };
    }

    bool is_overflow( int32_t value ) { return std::abs( value ) > MAX_SHIFT; }

    bool is_underflow( const chunks& value ) {
        return value.size() < MIN_CHUNKS;
    }

    bool is_out_of_bounds( const chunks& mantissa, int32_t shift ) {
        return is_overflow( shift ) || is_underflow( mantissa );
    }

    bool is_special( BigNumberType type ) {
        return type != BigNumberType::DEFAULT;
    }

    BigNumber
    make_special( BigNumberType type, const Error& error, bool is_negative ) {
        switch ( type ) {
        case BigNumberType::ZERO:
            return make_zero( error );

        case BigNumberType::NAN:
            return make_nan( error );

        case BigNumberType::INF:
            return make_inf( error, is_negative );

        default:
            if ( !is_ok( error ) ) return make_zero( error );
            return make_zero( make_error( ErrorCode::ERROR ) );
        }
    }

    BigNumber make_special( const chunks& mantissa,
                            int32_t shift,
                            BigNumberType type,
                            const Error& error,
                            bool is_negative ) {
        if ( is_special( type ) )
            return make_special( type, error, is_negative );

        if ( is_underflow( mantissa ) ) return make_zero( error );
        if ( is_overflow( shift ) ) return make_inf( error, is_negative );

        return make_zero( make_error( ErrorCode::ERROR ) );
    }

    chunks normalize( const chunks& raw_mantissa ) {
        return raw_mantissa; // TODO: Implement
    }

    int32_t normalize( int32_t raw_shift,
                       const chunks& raw_mantissa,
                       const chunks& norm_mantissa ) {
        return raw_shift; // TODO: Implement
    }

    BigNumber normalize( const chunks& mantissa,
                         int32_t shift,
                         BigNumberType type,
                         const Error& error,
                         bool is_negative ) {
        chunks norm_mantissa = normalize( mantissa );
        int32_t norm_shift = normalize( shift, mantissa, norm_mantissa );

        if ( is_out_of_bounds( norm_mantissa, norm_shift ) )
            return make_special(
                norm_mantissa, norm_shift, type, error, is_negative );

        return { norm_mantissa, norm_shift, type, error, is_negative };
    }

    BigNumber make_big_number( chunks mantissa,
                               int32_t shift,
                               BigNumberType type,
                               const Error& error,
                               bool is_negative ) noexcept {
        if ( is_special( type ) )
            return make_special( mantissa, shift, type, error, is_negative );

        return normalize( mantissa, shift, type, error, is_negative );
    }
}
