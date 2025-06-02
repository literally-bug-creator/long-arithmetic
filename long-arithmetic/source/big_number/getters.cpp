#include "getters.hpp"

#include <cstdint>

#include "big_number.hpp"

const int32_t ZERO = 0;

const Error& get_error( const BigNumber& number ) { return number.error; }

int32_t get_exponent( const BigNumber& number ) { return number.exponent; }

const std::vector<chunk>& get_chunks( const BigNumber& number );

size_t get_size( const BigNumber& number ) {
    return get_chunks( number ).size();
}

chunk get_chunk( const BigNumber& number, int32_t index ) {
    int32_t chunk_index = index - get_exponent( number );

    if ( chunk_index < ZERO || chunk_index >= get_size( number ) ) return ZERO;

    return get_chunks( number )[chunk_index];
}

chunk get_chunk_direct( const BigNumber& number, int32_t index ) {
    return get_chunks( number )[index];
}

bool is_negative( const BigNumber& number ) { return number.is_negative; }

bool is_zero( const BigNumber& number ) { return get_size( number ) == ZERO; }
