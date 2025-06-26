#include "big_number.hpp"
#include "constants.hpp"
#include "constructors.hpp"
#include "getters.hpp"

#define MULTIPLY_THRESHOLD 1000

namespace big_number {
    constexpr uint32_t MOD1 = 998244353;
    constexpr uint32_t MOD2 = 1004535809;
    constexpr uint32_t MOD3 = 469762049;
    constexpr uint32_t ROOT = 3;

    chunks remove_trailing_zeros( const chunks& value ) {
        auto last_non_zero =
            std::ranges::find_if( value.rbegin(), value.rend(), []( chunk c ) {
                return c != ZERO_INT;
            } );

        return chunks( value.begin(), last_non_zero.base() );
    }

    uint32_t mod_pow( uint64_t a, uint64_t e, uint32_t mod ) {
        uint64_t res = 1, base = a % mod;
        while ( e ) {
            if ( e & 1 ) res = ( res * base ) % mod;
            base = ( base * base ) % mod;
            e >>= 1;
        }
        return static_cast<uint32_t>( res );
    }

    static void ntt_mod( std::vector<uint32_t>& a, bool invert, uint32_t mod ) {
        size_t n = a.size();
        for ( size_t i = 1, j = 0; i < n; ++i ) {
            size_t bit = n >> 1;
            for ( ; j & bit; bit >>= 1 )
                j ^= bit;
            j |= bit;
            if ( i < j ) std::swap( a[i], a[j] );
        }

        for ( size_t len = 2; len <= n; len <<= 1 ) {
            uint32_t wlen = mod_pow( ROOT, ( mod - 1 ) / len, mod );
            if ( invert ) wlen = mod_pow( wlen, mod - 2, mod );

            for ( size_t i = 0; i < n; i += len ) {
                uint32_t w = 1;
                for ( size_t j = 0; j < ( len >> 1 ); ++j ) {
                    uint32_t u = a[i + j];
                    uint32_t v =
                        static_cast<uint64_t>( a[i + j + ( len >> 1 )] ) * w %
                        mod;

                    a[i + j] = u + v < mod ? u + v : u + v - mod;
                    a[i + j + ( len >> 1 )] = u >= v ? u - v : u + mod - v;
                    w = static_cast<uint64_t>( w ) * wlen % mod;
                }
            }
        }

        if ( invert ) {
            uint32_t inv_n = mod_pow( n, mod - 2, mod );
            for ( size_t i = 0; i < a.size(); ++i ) {
                a[i] = static_cast<uint64_t>( a[i] ) * inv_n % mod;
            }
        }
    }

    std::vector<uint32_t> to_base1e9( const chunks& c ) {
        std::vector<uint32_t> d;
        d.reserve( c.size() * 2 );
        for ( size_t i = 0; i < c.size(); ++i ) {
            d.push_back( c[i] % 1000000000ULL );
            d.push_back( c[i] / 1000000000ULL );
        }
        return d;
    }

    chunks from_ntt_crt3( const std::vector<uint32_t>& r1,
                          const std::vector<uint32_t>& r2,
                          const std::vector<uint32_t>& r3 ) {
        size_t n = r1.size();
        const __int128 m1 = MOD1, m2 = MOD2, m3 = MOD3;
        const __int128 m12 = m1 * m2;

        uint64_t inv_m1_mod2 =
            mod_pow( static_cast<uint64_t>( m1 % m2 ), m2 - 2, MOD2 );
        uint64_t inv_m12_mod3 =
            mod_pow( static_cast<uint64_t>( m12 % m3 ), m3 - 2, MOD3 );

        std::vector<__int128> coeff( n );
        for ( size_t i = 0; i < n; ++i ) {
            __int128 t = ( static_cast<__int128>( r2[i] ) - r1[i] + m2 ) % m2;
            t = t * inv_m1_mod2 % m2;
            __int128 x12 = r1[i] + t * m1;

            t = ( static_cast<__int128>( r3[i] ) - ( x12 % m3 ) + m3 ) % m3;
            t = t * inv_m12_mod3 % m3;
            coeff[i] = x12 + t * m12;
        }

        std::vector<uint32_t> digits;
        digits.reserve( n + 1 );
        __int128 carry = 0;
        for ( size_t i = 0; i < n; ++i ) {
            __int128 val = coeff[i] + carry;
            digits.push_back( static_cast<uint32_t>( val % 1000000000 ) );
            carry = val / 1000000000;
        }

        while ( carry > 0 ) {
            digits.push_back( static_cast<uint32_t>( carry % 1000000000 ) );
            carry /= 1000000000;
        }

        chunks out;
        out.reserve( digits.size() / 2 );
        for ( size_t i = 0; i < digits.size(); i += 2 ) {
            uint64_t low = digits[i];
            uint64_t high = ( i + 1 < digits.size() ) ? digits[i + 1] : 0;
            out.push_back( static_cast<chunk>( high ) * 1000000000ULL + low );
        }

        while ( !out.empty() && out.back() == 0 )
            out.pop_back();
        return out;
    }

    BigNumber ntt_mul( const BigNumber& A, const BigNumber& B ) {
        const Error error = propagate_error( A, B );

        if ( is_zero( A ) || is_zero( B ) ) return make_zero( error );

        std::vector<uint32_t> a = to_base1e9( get_mantissa( A ) );
        std::vector<uint32_t> b = to_base1e9( get_mantissa( B ) );

        size_t n = 1;
        while ( n < a.size() + b.size() )
            n <<= 1;

        a.resize( n, 0 );
        b.resize( n, 0 );

        std::vector<uint32_t> a1( a );
        std::vector<uint32_t> b1( b );
        std::vector<uint32_t> a2( a );
        std::vector<uint32_t> b2( b );
        std::vector<uint32_t> a3( a );
        std::vector<uint32_t> b3( b );

        ntt_mod( a1, false, MOD1 );
        ntt_mod( b1, false, MOD1 );
        ntt_mod( a2, false, MOD2 );
        ntt_mod( b2, false, MOD2 );
        ntt_mod( a3, false, MOD3 );
        ntt_mod( b3, false, MOD3 );

        for ( size_t i = 0; i < n; ++i ) {
            a1[i] = static_cast<uint64_t>( a1[i] ) * b1[i] % MOD1;
            a2[i] = static_cast<uint64_t>( a2[i] ) * b2[i] % MOD2;
            a3[i] = static_cast<uint64_t>( a3[i] ) * b3[i] % MOD3;
        }

        ntt_mod( a1, true, MOD1 );
        ntt_mod( a2, true, MOD2 );
        ntt_mod( a3, true, MOD3 );

        int32_t exp = get_shift( A ) + get_shift( B );
        bool sign = !has_same_sign( A, B );

        return make_big_number(
            std::move( remove_trailing_zeros( from_ntt_crt3( a1, a2, a3 ) ) ),
            exp,
            BigNumberType::DEFAULT,
            error,
            sign );
    }

    BigNumber simple_mul( const BigNumber& multiplicand,
                          const BigNumber& multiplier ) {
        const Error error = propagate_error( multiplicand, multiplier );

        chunks chunks( get_size( multiplicand ) + get_size( multiplier ), 0 );
        chunk carry = 0;
        for ( int32_t i = 0; i < get_size( multiplicand ); ++i ) {
            carry = 0;
            for ( int32_t j = 0; j < get_size( multiplier ); ++j ) {
                mul_chunk l_chunk = (mul_chunk)get_chunk( multiplicand, i );
                mul_chunk r_chunk = (mul_chunk)get_chunk( multiplier, j );
                mul_chunk product = l_chunk * r_chunk + chunks[i + j] + carry;
                carry = product / MAX_CHUNK;
                chunks[i + j] = product % MAX_CHUNK;
            }
            chunks[i + get_size( multiplier )] = carry;
        }

        return make_big_number( std::move( remove_trailing_zeros( chunks ) ),
                                get_shift( multiplicand ) +
                                    get_shift( multiplier ),
                                BigNumberType::DEFAULT,
                                error,
                                !has_same_sign( multiplicand, multiplier ) );
    }

    BigNumber
    mul_zero( const BigNumber& lhs, const BigNumber& rhs, const Error& error ) {
        switch ( get_type( rhs ) ) {
        case BigNumberType::DEFAULT:
            return make_zero( error );
        case BigNumberType::ZERO:
            return make_zero( error );
        case BigNumberType::INF:
            return make_nan( error );
        case BigNumberType::NOT_A_NUMBER:
            return make_nan( error );
        }
    }

    BigNumber
    mul_inf( const BigNumber& lhs, const BigNumber& rhs, const Error& error ) {
        switch ( get_type( rhs ) ) {
        case BigNumberType::DEFAULT:
            return make_inf( error, !has_same_sign( lhs, rhs ) );
        case BigNumberType::ZERO:
            return make_nan( error );
        case BigNumberType::INF:
            return make_inf( error, !has_same_sign( lhs, rhs ) );
        case BigNumberType::NOT_A_NUMBER:
            return make_nan( error );
        }
    }

    BigNumber mul_special( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( !is_special( lhs ) && is_special( rhs ) )
            return mul_special( rhs, lhs );

        const Error error = propagate_error( lhs, rhs );

        switch ( get_type( lhs ) ) {
        case BigNumberType::NOT_A_NUMBER:
            return make_nan( error );
        case BigNumberType::INF:
            return mul_inf( lhs, rhs, error );
        case BigNumberType::ZERO:
            return mul_zero( lhs, rhs, error );
        case BigNumberType::DEFAULT:
            return ntt_mul( lhs, rhs );
        }
    }

    BigNumber mul( const BigNumber& lhs, const BigNumber& rhs ) {
        if ( is_special( lhs ) || is_special( rhs ) )
            return mul_special( lhs, rhs );

        if ( static_cast<int>( get_size( lhs ) ) <= MULTIPLY_THRESHOLD ||
             static_cast<int>( get_size( rhs ) ) <= MULTIPLY_THRESHOLD ) {
            return simple_mul( lhs, rhs );
        }
        return ntt_mul( lhs, rhs );
    }
}
