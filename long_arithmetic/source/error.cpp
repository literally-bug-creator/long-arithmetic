#include "error.hpp"

namespace big_number {
    Error make_error( ErrorCode code ) { return Error{ code }; }

    const ErrorCode& get_error_code( const Error& error ) { return error.code; }

    bool is_ok( const Error& error ) {
        ErrorCode code = get_error_code( error );
        return code == ErrorCode::OK;
    }

    Error get_default_error() { return Error{ OK }; }
}
