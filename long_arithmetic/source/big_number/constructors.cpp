#include "constructors.hpp"

#include "big_number.hpp"

namespace big_number {
    chunk ZERO_CHUNK = 0;

    int32_t compute_exponent_delta( const std::vector<chunk>& chunks ) {
        size_t first_non_zero_chunk = chunks.size();
        size_t last_non_zero_chunk = 0;

        for ( size_t index = 0; index < chunks.size(); index++ ) {
            chunk value = chunks[index];
            if ( value == ZERO_CHUNK ) continue;

            if ( index < first_non_zero_chunk ) first_non_zero_chunk = index;
            if ( index > last_non_zero_chunk ) last_non_zero_chunk = index;
        }

        return ( first_non_zero_chunk + 1 ) -
               ( chunks.size() - ( last_non_zero_chunk + 1 ) );
    }

    std::vector<chunk> trim_zeros( std::vector<chunk>& chunks ) {
        if ( chunks.empty() ) return {};

        size_t size = chunks.size();
        size_t first_nonzero = 0;
        size_t last_nonzero = size;

        while ( first_nonzero < size && chunks[first_nonzero] == 0 ) {
            first_nonzero++;
        }

        if ( first_nonzero == size ) return {};

        while ( last_nonzero > first_nonzero && chunks[last_nonzero - 1] == 0 )
            last_nonzero--;

        return std::vector<chunk>(
            std::make_move_iterator( chunks.begin() + first_nonzero ),
            std::make_move_iterator( chunks.begin() + last_nonzero ) );
    }

    BigNumber from_scratch( std::vector<chunk>& chunks,
                            int32_t exponent,
                            bool is_negative,
                            const Error& error = DEFAULT_ERROR ) {
        int32_t exponent_delta = compute_exponent_delta( chunks );
        std::vector<chunk> trimmed_chunks = trim_zeros( chunks );

        if ( trimmed_chunks.empty() ) { return make_zero(); }

        return BigNumber(
            trimmed_chunks, exponent + exponent_delta, is_negative, error );
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
