#include "raw_number.hpp"

#include <cstdint>

#include "big_number.hpp"

namespace big_number {

    int32_t compute_offset( int32_t exp ) noexcept {
        int32_t remainder = exp % BASE;
        return ( remainder < 0 ) ? remainder + BASE : remainder;
    }

    size_t count_trailing_zeros( const std::vector<int>& digits ) noexcept {
        size_t count = 0;
        for ( auto it = digits.rbegin(); it != digits.rend(); ++it ) {
            if ( *it != 0 ) break;
            ++count;
        }
        return count;
    }

    size_t count_leading_zeros( const std::vector<int>& digits ) noexcept {
        size_t count = 0;
        for ( auto it = digits.begin(); it != digits.end(); ++it ) {
            if ( *it != 0 ) break;
            ++count;
        }
        return count;
    }

    RawNumber make_raw_number( const std::vector<int>& digits,
                               int32_t exp,
                               bool is_negative,
                               const Error& error ) {
        const size_t leading_zeros = count_leading_zeros( digits );
        const size_t trailing_zeros = count_trailing_zeros( digits );
        const int32_t offset = compute_offset( exp );

        return RawNumber( digits,
                          error,
                          exp,
                          leading_zeros,
                          trailing_zeros,
                          offset,
                          is_negative );
    }

    size_t get_size( const RawNumber& raw ) { return raw.digits.size(); }

    int32_t get_digit( const RawNumber& raw, size_t index ) {
        if ( index >= get_size( raw ) ) return 0;
        return raw.digits[index];
    }

    int32_t get_exp( const RawNumber& raw ) { return raw.exp; }

    int32_t get_leading_zeros( const RawNumber& raw ) {
        return raw.leading_zeros;
    }

    int32_t get_trailing_zeros( const RawNumber& raw ) {
        return raw.trailing_zeros;
    }

    int32_t get_offset( const RawNumber& raw ) { return raw.offset; }

    bool is_negative( const RawNumber& raw ) { return raw.is_negative; }

    const Error& get_error( const RawNumber& raw ) { return raw.error; }
}
