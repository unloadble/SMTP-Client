
#ifndef SMTP_CLIENT_CLOSING_ERROR_H
#define SMTP_CLIENT_CLOSING_ERROR_H

#include "smtp_error.h"
namespace smtp::exceptions {
    class ClosingError : public SmtpError {
    public:
        const char * what() const noexcept override;
    };
}
#endif //SMTP_CLIENT_CLOSING_ERROR_H
