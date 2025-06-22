#include "big_number.hpp"

using namespace big_number;

BigNumber create_big_number( const chunks& chunks,
                           int32_t shift,
                           bool is_negative = false,
                           BigNumberType type = BigNumberType::DEFAULT );
