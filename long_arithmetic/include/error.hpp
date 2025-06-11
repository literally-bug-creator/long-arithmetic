#pragma once

#include <string>

namespace big_number {
    enum ErrorCode {
        OK,
        CALCULATION_ERROR,
    };

    struct Error {
        ErrorCode code;
        std::string message;
    };

    Error make_error( ErrorCode code, const std::string& message );

    const ErrorCode& get_error_code( const Error& error );

    const std::string& get_error_message( const Error& error );

    bool is_ok( const Error& error );

    Error get_default_error();
}
