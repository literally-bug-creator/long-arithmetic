#include "constructors.hpp"

#include "big_number.hpp"

namespace big_number {
    chunk ZERO_CHUNK = ZERO_INT;

    BigNumber make_zero( const Error& error ) {
        return BigNumber( {}, ZERO_INT, false, error );
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
                chunks[last_nonzero - ONE_INT] == ZERO_INT )
            last_nonzero--;

        return std::vector<chunk>(
            std::make_move_iterator( chunks.begin() + first_nonzero ),
            std::make_move_iterator( chunks.begin() + last_nonzero ) );
    }

    int32_t compute_exponent_shift( int32_t exponent ) {
        return ( ( exponent % BASE ) + BASE ) % BASE;
    }

    std::vector<int> normalize_digits( std::vector<int> digits,
                                       int32_t exponent ) {
        int32_t shift = compute_exponent_shift( exponent );
        digits.reserve( digits.size() + shift );
        digits.insert( digits.end(), shift, 0 );
        return digits;
    }

    int32_t normalize_exponent( int32_t exp ) {
        int32_t shift = compute_exponent_shift( exp );
        int32_t unsigned_exp = ( ::abs( exp - shift ) / BASE );
        return unsigned_exp * ( exp < 0 ? -1 : 1 );
    }

    std::vector<chunk> convert_to_chunks( std::vector<int> digits ) {
        if ( digits.empty() || BASE <= 0 ) return {};

        std::vector<chunk> chunks;
        chunks.reserve( digits.size() / BASE + 1 );
        const int total_digits = digits.size();

        for ( int i = total_digits; i > 0; i -= BASE ) {
            int start = std::max( 0, i - BASE );
            int end = i;

            chunk current_chunk = 0;
            chunk multiplier = 1;

            for ( int j = end - 1; j >= start; --j ) {
                current_chunk += digits[j] * multiplier;
                multiplier *= 10;
            }

            chunks.push_back( current_chunk );
        }

        return chunks;
    }

    BigNumber make_big_number( std::vector<int> digits,
                               int32_t exponent,
                               bool is_negative,
                               const Error& error ) {
        if ( ( digits.size() == 0 ) || ( digits[0] == 0 ) ) {
            return make_zero();
        }

        std::vector<int> normalized_digits =
            normalize_digits( digits, exponent );

        int32_t normalized_exp = normalize_exponent( exponent );
        std::vector<chunk> chunks = convert_to_chunks( normalized_digits );

        return internal_make_big_number(
            chunks, normalized_exp, is_negative, error );
    }

    BigNumber internal_make_big_number( std::vector<chunk> chunks,
                                        int32_t exponent,
                                        bool is_negative,
                                        const Error& error ) {
        std::vector<chunk> trimmed_chunks = trim_zeros( chunks );
        if ( trimmed_chunks.empty() ) { return make_zero(); }

        return BigNumber( trimmed_chunks, exponent, is_negative, error );
    }
}
