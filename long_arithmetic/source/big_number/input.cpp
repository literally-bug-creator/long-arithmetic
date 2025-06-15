#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <sys/types.h>

#include "big_number.hpp"
#include "constructors.hpp"

namespace big_number {
    size_t count_chunks( const std::vector<uint8_t>& digits, uint8_t offset ) {
        if ( digits.size() == 0 ) return 0;
        if ( digits[0] == 0 ) return 0;
        size_t amount = ( digits.size() + static_cast<size_t>( offset ) +
                          static_cast<size_t>( BASE ) - 1 ) /
                        BASE;

        return ( amount > static_cast<size_t>( MAX_CHUNK_AMOUNT ) ) ? 0
                                                                    : amount;
    }

    uint8_t get_digit( const std::vector<uint8_t>& digits, size_t index ) {
        if ( index >= digits.size() ) return 0;
        return digits[index];
    }

    int32_t compute_shift( int32_t exp, uint8_t offset ) {
        return ( exp - static_cast<int32_t>( offset ) ) /
               static_cast<int32_t>( BASE );
    }

    std::vector<chunk> convert_to_chunks( const std::vector<uint8_t>& digits,
                                          uint8_t offset ) {
        std::vector<chunk> chunks( count_chunks( digits, offset ) );
        int32_t index = static_cast<int32_t>( digits.size() ) +
                        static_cast<int32_t>( offset ) - 1;

        for ( chunk& value : chunks ) {
            int32_t to = std::max( -1, index - BASE );
            chunk factor = 1;

            for ( int32_t i = index; i > to; i-- ) {
                int32_t digit = get_digit( digits, i );
                value += static_cast<chunk>( digit ) * factor;
                factor *= 10;
            }
            index = to;
        }
        return chunks;
    }

    uint8_t compute_offset( int32_t exp ) {
        int8_t remainder = static_cast<uint8_t>( exp % BASE );
        return ( remainder < 0 ) ? remainder + BASE : remainder;
    }

    BigNumber make_big_number( const std::vector<uint8_t>& digits,
                               int32_t exp,
                               bool is_negative,
                               const Error& error ) {
        uint8_t offset = compute_offset( exp );
        std::vector<chunk> chunks = convert_to_chunks( digits, offset );
        int32_t shift = compute_shift( exp, offset );

        return chunks.empty() ? make_zero( error )
                              : BigNumber{ chunks, error, shift, is_negative };
    }
}
