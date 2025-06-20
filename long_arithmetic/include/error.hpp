#pragma once

#include <cstdint>

namespace big_number {
    enum class ErrorCode : uint8_t {
        OK,
        ERROR,
    };

    struct Error {
        ErrorCode code;
    };

    Error make_error( ErrorCode code );

    const ErrorCode& get_error_code( const Error& error );

    bool is_ok( const Error& error );

    Error get_default_error();
}
