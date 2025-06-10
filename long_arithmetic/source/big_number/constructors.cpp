#include "constructors.hpp"

namespace big_number {
    chunk ZERO_CHUNK = ZERO_INT;

    int32_t compute_exponent_delta( const std::vector<chunk>& chunks ) {
        size_t first_non_zero_chunk = chunks.size();
        size_t last_non_zero_chunk = ZERO_INT;

        for ( size_t index = ZERO_INT; index < chunks.size(); index++ ) {
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
        size_t first_nonzero = ZERO_INT;
        size_t last_nonzero = size;

        while ( first_nonzero < size && chunks[first_nonzero] == ZERO_INT ) {
            first_nonzero++;
        }

        if ( first_nonzero == size ) return {};

        while ( last_nonzero > first_nonzero &&
                chunks[last_nonzero - 1] == ZERO_INT )
            last_nonzero--;

        return std::vector<chunk>(
            std::make_move_iterator( chunks.begin() + first_nonzero ),
            std::make_move_iterator( chunks.begin() + last_nonzero ) );
    }

    BigNumber make_zero( const Error& error ) {
        return BigNumber( {}, ZERO_INT, false, error );
    }

    BigNumber make_big_number( std::vector<chunk> chunks,
                               int32_t exponent,
                               bool is_negative,
                               const Error& error ) {
        int32_t exponent_delta = compute_exponent_delta( chunks );
        std::vector<chunk> trimmed_chunks = trim_zeros( chunks );

        if ( trimmed_chunks.empty() ) { return make_zero(); }

        return BigNumber(
            trimmed_chunks, exponent + exponent_delta, is_negative, error );
    }
}
