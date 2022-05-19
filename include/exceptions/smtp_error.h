
#ifndef SMTP_CLIENT_SMTP_ERROR_H
#define SMTP_CLIENT_SMTP_ERROR_H

#include <exception>
namespace smtp::exceptions {
    class SmtpError : public std::exception { };
}

#endif //SMTP_CLIENT_SMTP_ERROR_H
