#include "constructors.hpp"

#include <cstdint>
#include <cstring>

#include "error.hpp"

void strip_zeros( BigNumber& num ) { // TODO: reimplement
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

BigNumber make_big_number( const std::string& number ) { // TODO: reimplement
    BigNumber res;
    res.chunks.clear();
    res.exponent = 0;
    res.is_negative = false;
    res.error = Error();
    std::string num = number;
    int64_t expAdjust = 0;
    auto ePos = num.find_first_of( "eE" );
    if ( ePos != std::string::npos ) {
        std::string expPart = num.substr( ePos + 1 );
        expAdjust = std::stoll( expPart );
        num.erase( ePos );
    }
    if ( !num.empty() && num[0] == '-' ) {
        res.is_negative = true;
        num.erase( 0, 1 );
    }
    size_t frac_len = 0;
    auto dot = num.find( '.' );
    if ( dot != std::string::npos ) {
        frac_len = num.size() - dot - 1;
        num.erase( dot, 1 );
    }
    if ( expAdjust >= 0 ) {
        if ( expAdjust > (int64_t)frac_len ) {
            num.append( expAdjust - frac_len, '0' );
            frac_len = 0;
        } else {
            frac_len -= expAdjust;
        }
    } else {
        num = std::string( -expAdjust, '0' ) + num;
        frac_len += static_cast<size_t>( -expAdjust );
    }
    num.erase( 0, num.find_first_not_of( '0' ) );
    if ( num.empty() ) num = "0";
    res.exponent =
        -( frac_len / CHUNK_DIGITS + ( frac_len % CHUNK_DIGITS != 0 ) );
    if ( frac_len > 0 ) {
        num += std::string( -res.exponent * CHUNK_DIGITS - frac_len, '0' );
    }
    size_t chunks_count =
        num.size() / CHUNK_DIGITS + ( num.size() % CHUNK_DIGITS != 0 );
    num = std::string( chunks_count * CHUNK_DIGITS - num.size(), '0' ) + num;
    res.chunks.resize( chunks_count );
    for ( size_t i = 0; i < chunks_count; i++ ) {
        size_t start = num.size() - ( i + 1 ) * CHUNK_DIGITS;
        res.chunks[i] = std::stoull( num.substr( start, CHUNK_DIGITS ) );
    }
    strip_zeros( res );
    return res;
}

BigNumber from_scratch( const std::vector<chunk>& chunks,
                        int32_t exponent,
                        bool is_negative,
                        const Error& error ) { // TODO: reimplement

    BigNumber res;
    res.chunks = chunks;
    res.exponent = exponent;
    res.is_negative = is_negative;
    res.error = error;
    strip_zeros( res );
    return res;
}

BigNumber from_iterator( std::vector<chunk>::const_iterator begin,
                         std::vector<chunk>::const_iterator end ) {
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
    return from_scratch(
        chunks, exponent, false, make_error( OK, "" ) ); // TODO: Fix
}

BigNumber make_zero( Error error ) {
    return from_scratch( {}, 0, false, error );
}

BigNumber from_int( int v ) { return make_big_number( std::to_string( v ) ); }

BigNumber from_long( long v ) { return make_big_number( std::to_string( v ) ); }

BigNumber from_long_long( long long v ) {
    return make_big_number( std::to_string( v ) );
}

BigNumber from_float( float v ) {
    return make_big_number( std::to_string( v ) );
}

BigNumber from_double( double v ) {
    return make_big_number( std::to_string( v ) );
}

std::string to_string( const BigNumber& number ) { // TODO: reimplement
    if ( number.chunks.empty() ||
         ( number.chunks.size() == 1 && number.chunks[0] == 0 ) )
        return "0";

    std::string result;
    if ( number.is_negative ) { result.push_back( '-' ); }

    size_t chunks = number.chunks.size();
    int32_t exp = number.exponent;
    int32_t max_exp = std::max( exp + static_cast<int32_t>( chunks ) - 1, 0 );
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
                std::string( CHUNK_DIGITS - chunk_str.size(), '0' ) + chunk_str;
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
        if ( !result.empty() && result.back() == '.' ) { result.pop_back(); }
    }

    return result;
}
