
#include "exceptions/auth_error.h"
namespace smtp::exceptions {
    AuthError::AuthError(AuthErrorType error_type) : error_type {error_type} {}

    const char * AuthError::what() const noexcept {
        switch (error_type) {
            case AuthErrorType::AUTH_COMMAND:
                return "Failed AUTH Command !?";
            case AuthErrorType::INVALID_USERNAME:
                return "Invalid Username !?";
            case AuthErrorType::INVALID_AUTHENTICATION:
                return "Invalid Username or Password !?";
            default:
                return "AuthError !?";
        }
    }
}
