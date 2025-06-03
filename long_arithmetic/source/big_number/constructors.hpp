#include "big_number.hpp"

BigNumber make_big_number( const std::string& str );

BigNumber from_scratch( const std::vector<chunk>& chunks,
                        int32_t exponent,
                        bool is_negative,
                        const Error& error );

BigNumber from_iterator( std::vector<chunk>::const_iterator begin,
                         std::vector<chunk>::const_iterator end );

BigNumber make_zero();

BigNumber from_int(int v);

BigNumber from_long(long v);

BigNumber from_long_long(long long v);

BigNumber from_float(float v);

BigNumber from_double(double v);

std::string to_string( const BigNumber& number );
