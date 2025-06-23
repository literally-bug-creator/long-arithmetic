#pragma once

#include "big_number.hpp"

namespace big_number {
    BigNumber make_big_number( chunks chunks,
                               int32_t shift,
                               BigNumberType type,
                               const Error& error,
                               bool is_negative );
}
