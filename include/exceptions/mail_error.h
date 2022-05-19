#ifndef SMTP_CLIENT_MAIL_ERROR_H
#define SMTP_CLIENT_MAIL_ERROR_H

#include "smtp_error.h"
namespace smtp::exceptions {
    enum class MailErrorType {
        INVALID_FROM_EMAIL,
        INVALID_RCPT_EMAIL,
        FAILED_DATA_COMMAND,
        INVALID_BODY
    };


    class MailError : public SmtpError {
    private:
        MailErrorType error_type;
    public:
        MailError() = default;
        MailError(MailErrorType error_type);

        const char * what() const noexcept override;

    };
}
#endif //SMTP_CLIENT_MAIL_ERROR_H
