
#include "exceptions/mail_error.h"

namespace smtp::exceptions {
    MailError::MailError(MailErrorType error_type) : error_type {error_type} {}

    const char * MailError::what() const noexcept {
        switch (error_type) {
            case MailErrorType::INVALID_FROM_EMAIL:
                return "Invalid FROM Email.";
            case MailErrorType::INVALID_RCPT_EMAIL:
                return "INVALID RCPT Email";
            case MailErrorType::FAILED_DATA_COMMAND:
                return "Failed Data Command.";
            case MailErrorType::INVALID_BODY:
                return "Invalid Body Mail.";
            default:
                return "MailError !?";
        }
    }
}