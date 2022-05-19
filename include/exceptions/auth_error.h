
#ifndef SMTP_CLIENT_AUTH_ERROR_H
#define SMTP_CLIENT_AUTH_ERROR_H

#include "smtp_error.h"
namespace smtp::exceptions {
    enum class AuthErrorType {
        AUTH_COMMAND,
        INVALID_USERNAME,
        INVALID_AUTHENTICATION
    };

    class AuthError : public SmtpError {
    private:
        AuthErrorType error_type;
    public:
        AuthError() = default;
        AuthError(AuthErrorType error_type);

        const char * what() const noexcept override;
    };
}

#endif //SMTP_CLIENT_AUTH_ERROR_H
