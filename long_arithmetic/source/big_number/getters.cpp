#include "getters.hpp"

#include "big_number.hpp"

namespace big_number {
    const int32_t ZERO_INT = 0;

    const Error& get_error( const BigNumber& number ) { return number.error; }

    int32_t get_shift( const BigNumber& number ) { return number.shift; }

    const std::vector<chunk>& get_chunks( const BigNumber& number ) {
        return number.chunks;
    }

    size_t get_size( const BigNumber& number ) {
        return get_chunks( number ).size();
    }

    chunk get_chunk( const BigNumber& number, int32_t index ) {
        int32_t chunk_index = index - get_shift( number );

        if ( chunk_index < ZERO_INT || chunk_index >= get_size( number ) )
            return ZERO_INT;

        return get_chunks( number )[chunk_index];
    }

    bool is_negative( const BigNumber& number ) { return number.is_negative; }

    bool is_zero( const BigNumber& number ) {
        return get_size( number ) == ZERO_INT;
    }

}
