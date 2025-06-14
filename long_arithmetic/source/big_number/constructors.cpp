#include "constructors.hpp"

#include "big_number.hpp"

namespace big_number {
    int32_t ZERO_INT = 0;

    BigNumber make_zero( const Error& error ) {
        return BigNumber( {}, error, ZERO_INT, false );
    }
}
