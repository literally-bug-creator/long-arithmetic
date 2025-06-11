#include "error.hpp"

namespace big_number {
    const std::string EMPTY_STR = "";

    Error make_error( ErrorCode code, const std::string& message ) {
        return Error{ code, message };
    }

    const ErrorCode& get_error_code( const Error& error ) { return error.code; }

    const std::string& get_error_message( const Error& error ) {
        return error.message;
    }

    bool is_ok( const Error& error ) {
        ErrorCode code = get_error_code( error );
        return code == ErrorCode::OK;
    }

    Error get_default_error() { return Error{ OK, EMPTY_STR }; }
}
