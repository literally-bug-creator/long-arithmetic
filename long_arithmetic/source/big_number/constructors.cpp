#include "constructors.hpp"

#include <cstdint>
#include <cstring>

#include "error.hpp"

namespace big_number {
    void strip_zeros( BigNumber& num ) { // TODO: need a refactor
        size_t pref_zeros = 0;
        size_t suff_zeros = 0;
        size_t sz = num.chunks.size();
        for ( size_t i = 0; i < sz; ++i ) {
            if ( num.chunks[i] == 0 )
                pref_zeros++;
            else
                break;
        }
        for ( size_t i = sz; i > 0; --i ) {
            if ( num.chunks[i - 1] == 0 )
                suff_zeros++;
            else
                break;
        }
        if ( pref_zeros == sz ) {
            num.exponent = 0;
            num.is_negative = false;
            num.chunks.clear();
            return;
        }
        num.exponent += pref_zeros;
        size_t new_size = sz - pref_zeros - suff_zeros;
        if ( new_size == sz ) return;
        num.chunks.erase( num.chunks.begin(), num.chunks.begin() + pref_zeros );
        num.chunks.resize( new_size );
    }

    BigNumber from_scratch(
        const std::vector<chunk>& chunks,
        int32_t exponent,
        bool is_negative,
        const Error& error = DEFAULT_ERROR ) { // TODO: need a refactor

        BigNumber res;
        res.chunks = chunks;
        res.exponent = exponent;
        res.is_negative = is_negative;
        res.error = error;
        strip_zeros( res );
        return res;
    }

    BigNumber from_iterator(
        std::vector<chunk>::const_iterator begin,
        std::vector<chunk>::const_iterator end ) { // TODO: need a refactor
        std::vector<chunk> chunks;
        int32_t exponent = 0;
        size_t new_size = end - begin;
        while ( new_size > 0 && *begin == 0 ) {
            ++begin;
            --new_size;
            exponent++;
        }
        while ( new_size > 0 && *( end - 1 ) == 0 ) {
            --end;
            new_size--;
        }
        if ( new_size > 0 ) { chunks = std::vector<chunk>( begin, end ); }
        return from_scratch( chunks, exponent, false );
    }

    BigNumber make_zero( Error error ) {
        return from_scratch( {}, 0, false, error );
    }

    std::string to_string( const BigNumber& number ) { // TODO: need a refactor
        if ( number.chunks.empty() ||
             ( number.chunks.size() == 1 && number.chunks[0] == 0 ) )
            return "0";

        std::string result;
        if ( number.is_negative ) { result.push_back( '-' ); }

        size_t chunks = number.chunks.size();
        int32_t exp = number.exponent;
        int32_t max_exp =
            std::max( exp + static_cast<int32_t>( chunks ) - 1, 0 );
        int32_t min_exp = std::min( exp, 0 );

        for ( int32_t i = max_exp; i >= min_exp; --i ) {
            if ( i == -1 ) { result += '.'; }
            std::string chunk_str;
            int32_t chunk_idx = i - exp;
            if ( chunk_idx >= 0 && static_cast<size_t>( chunk_idx ) < chunks ) {
                chunk_str = std::to_string( number.chunks[chunk_idx] );
            } else {
                chunk_str = "0";
            }
            if ( i != max_exp ) {
                chunk_str =
                    std::string( CHUNK_DIGITS - chunk_str.size(), '0' ) +
                    chunk_str;
            }
            result += chunk_str;
        }

        if ( min_exp < 0 ) {
            size_t suff_zeros = 0;
            for ( size_t i = result.size(); i > 0; --i ) {
                if ( result[i - 1] == '0' ) {
                    ++suff_zeros;
                } else if ( result[i - 1] == '.' ) {
                    break;
                } else {
                    break;
                }
            }
            if ( suff_zeros > 0 ) {
                result.erase( result.size() - suff_zeros, suff_zeros );
            }
            if ( !result.empty() && result.back() == '.' ) {
                result.pop_back();
            }
        }

        return result;
    }
}
