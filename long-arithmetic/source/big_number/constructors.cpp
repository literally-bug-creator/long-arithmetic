#include "constructors.hpp"

#include <cstddef>
#include <cstdint>
#include <regex>
#include <string>
#include <vector>

#include "big_number.hpp"
#include "error.hpp"
#include "getters.hpp"

const std::string ZERO_STR = "0";

std::string normalize_scientific( const std::string& s ) {
    std::smatch match;
    std::regex sci_regex( R"(^([+-]?)(\d*)(\.(\d*))?[eE]([+-]?\d+)$)" );
    if ( !regex_match( s, match, sci_regex ) ) return s;
    std::string sign = match[1];
    std::string int_part = match[2];
    std::string frac_part = match[4];
    int exp = stoi( match[5] );
    std::string digits = int_part + frac_part;
    int point_pos = int_part.size();
    int new_point = point_pos + exp;
    if ( new_point < 0 ) {
        digits = std::string( -new_point, '0' ) + digits;
        new_point = 0;
    }
    if ( new_point > (int)digits.size() ) {
        digits += std::string( new_point - digits.size(), '0' );
        new_point = digits.size();
    }
    std::string res = sign + digits.substr( 0, new_point );
    if ( new_point < (int)digits.size() ) {
        res += "." + digits.substr( new_point );
    }
    if ( !res.empty() && res[0] == '+' ) res.erase( 0, 1 );
    return res;
}

void parse_sign( std::string& num, bool is_negative ) {
    if ( num[0] == '-' ) {
        is_negative = true;
        num.erase( 0, 1 );
    } else if ( num[0] == '+' ) {
        num.erase( 0, 1 );
    }
}

void parse_fraction( std::string& num, int32_t& exponent ) {
    size_t dot = num.find( '.' );
    if ( dot != std::string::npos ) {
        size_t frac_size = num.size() - dot - 1;
        exponent =
            -( frac_size / CHUNK_DIGITS + ( frac_size % CHUNK_DIGITS != 0 ) );
        num.erase( dot, 1 );
        num += std::string( -exponent * CHUNK_DIGITS - frac_size, '0' );
    }
}

void remove_leading_zeros( std::string& num ) {
    num.erase( 0, num.find_first_not_of( '0' ) );
}

void remove_leading_chunk_zeros( std::vector<chunk>& chunks,
                                 int32_t& exponent ) {
    size_t leading_zeros = 0;
    while ( leading_zeros < chunks.size() && chunks[leading_zeros] == 0 ) {
        ++leading_zeros;
    }
    exponent += leading_zeros;
    chunks.erase( chunks.begin(), chunks.begin() + leading_zeros );
}

BigNumber make_big_number( const std::string& str ) {
    std::string num = normalize_scientific( str );
    if ( num.empty() ) {
        return from_scratch(
            {}, 0, false, make_error( INVALID_INPUT, "Empty string" ) );
    }
    bool is_negative = false;
    parse_sign( num, is_negative );
    int32_t exponent = 0;
    parse_fraction( num, exponent );
    remove_leading_zeros( num );
    if ( num.empty() ) { return make_zero(); }
    size_t chunks_count =
        num.size() / CHUNK_DIGITS + ( num.size() % CHUNK_DIGITS != 0 );
    num = std::string( chunks_count * CHUNK_DIGITS - num.size(), '0' ) + num;
    std::vector<chunk> chunks( chunks_count );
    for ( size_t i = 0; i < chunks_count; i++ ) {
        size_t start = num.size() - ( i + 1 ) * CHUNK_DIGITS;
        chunks[i] = stoull( num.substr( start, CHUNK_DIGITS ) );
    }
    remove_leading_chunk_zeros( chunks, exponent );
    if ( chunks.empty() ) { return make_zero(); }
    return from_scratch( chunks, exponent, is_negative, make_error( OK, "" ) );
}

BigNumber from_scratch( const std::vector<chunk>& chunks,
                        int32_t exponent,
                        bool is_negative,
                        const Error& error ) {
    return BigNumber{ .chunks = chunks,
                      .exponent = exponent,
                      .is_negative = is_negative,
                      .error = error };
}

BigNumber from_iterator( std::vector<chunk>::const_iterator begin,
                         std::vector<chunk>::const_iterator end ) {
    size_t size = end - begin;
    int32_t exponent = 0;
    std::vector<chunk> chunks( 0 );

    while ( size > 0 && *begin == 0 ) {
        ++begin;
        --size;
        ++exponent;
    }

    while ( size > 0 && *( end - 1 ) == 0 ) {
        --end;
        size--;
    }
    if ( size > 0 ) { chunks = std::vector<chunk>( begin, end ); }

    return from_scratch( chunks, exponent, false, make_error( OK, "" ) );
}

BigNumber make_zero() {
    return from_scratch( {}, 0, false, make_error( OK, "" ) );
}

std::string sign_to_string( const BigNumber& number ) {
    return is_negative( number ) ? "-" : "";
}

std::string chunk_to_string( const BigNumber& number, size_t index ) {
    std::string str = std::to_string( get_chunk_direct( number, index ) );
    str = std::string( CHUNK_DIGITS - str.length(), '0' ) + str;
    return str;
}

std::string chunks_to_string( const BigNumber& number ) {
    std::string str =
        std::to_string( get_chunk_direct( number, get_size( number ) - 1 ) );

    for ( size_t i = get_size( number ) - 2; i >= 0; --i ) {
        str += chunk_to_string( number, i );
    }

    return str;
}

std::string to_string( const BigNumber& number ) {
    if ( is_zero( number ) ) return ZERO_STR;

    std::string str = sign_to_string( number ) + chunks_to_string( number );

    if ( get_exponent( number ) >= 0 ) {
        str += std::string( get_exponent( number ) * CHUNK_DIGITS, '0' );
    } else {
        int dot_pos = str.length() + get_exponent( number ) * CHUNK_DIGITS;
        if ( dot_pos <= 0 ) {
            str = "0." + std::string( -dot_pos, '0' ) + str;
        } else {
            str.insert( dot_pos, "." );
        }
    }
    return str;
}
