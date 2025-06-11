#include "constructors.hpp"

#include "big_number.hpp"

namespace big_number {
    chunk ZERO_CHUNK = ZERO_INT;

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
                chunks[last_nonzero - ONE_INT] == ZERO_INT )
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
        std::vector<chunk> trimmed_chunks = trim_zeros( chunks );
        if ( trimmed_chunks.empty() ) { return make_zero(); }

        return BigNumber( trimmed_chunks, exponent, is_negative, error );
    }
}
