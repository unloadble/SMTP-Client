
#ifndef SMTP_CLIENT_HANDSHAKING_ERROR_H
#define SMTP_CLIENT_HANDSHAKING_ERROR_H

#include "smtp_error.h"
namespace smtp::exceptions {
    class HandshakingError : public SmtpError {
    public:
        const char * what() const noexcept override;
    };
}
#endif //SMTP_CLIENT_HANDSHAKING_ERROR_H
