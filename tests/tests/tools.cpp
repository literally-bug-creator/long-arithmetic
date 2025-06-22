#include "tools.hpp"

#include "big_number.hpp"
#include "error.hpp"

BigNumber create_big_number( const chunks& mantissa,
                           int32_t shift,
                           bool is_negative,
                           BigNumberType type ) {
    return BigNumber( mantissa, shift, type, get_default_error(), is_negative );
}
