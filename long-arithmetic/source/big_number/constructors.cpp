#include "big_number.hpp"

BigNumber make_big_number( const std::string& str );

BigNumber from_scratch( std::vector<chunk>& chunks,
                        int32_t exponent,
                        bool is_negative,
                        Error& error ) {
    return BigNumber{ .chunks = chunks,
                      .exponent = exponent,
                      .is_negative = is_negative,
                      .error = error };
}

std::string to_string( const BigNumber& number );
