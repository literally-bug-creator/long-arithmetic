#include "tools.hpp"

BigNumber create_big_number( const chunks& chunks,
                             int32_t shift,
                             bool is_negative,
                             big_number::BigNumberType type ) {
    big_number::BigNumber number;
    number.mantissa = chunks;
    number.shift = shift;
    number.is_negative = is_negative;
    number.type = type;
    number.error = big_number::Error{};
    return number;
}
