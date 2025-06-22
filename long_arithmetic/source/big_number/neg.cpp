#include "big_number.hpp"
#include "constructors.hpp"
#include "getters.hpp"

namespace big_number {
    BigNumber neg( const BigNumber& number ) {
        return make_big_number( get_mantissa( number ),
                                get_shift( number ),
                                get_type( number ),
                                get_error( number ),
                                !is_negative( number ) );
    }
}
