
#ifndef SMTP_CLIENT_SMTP_SERVER_H
#define SMTP_CLIENT_SMTP_SERVER_H

#include <iostream>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "sys/socket.h"
#include <cerrno>
#include "netdb.h"
#include "unistd.h"
#include <cstring>
#include <thread>
#include <sstream>
#include "encoders/base64.h"
#include "constants/smtp_constants.h"
#include "exceptions/connection_error.h"
#include "exceptions/closing_error.h"
#include "exceptions/sending_error.h"
#include "exceptions/handshaking_error.h"
#include "exceptions/ssl_error.h"
#include "exceptions/mail_error.h"
#include "exceptions/auth_error.h"


namespace smtp {
    using namespace constants;
    using namespace exceptions;
    using namespace encoder;

    class SmtpServer {
    private:
        const std::string END_FRAMING_MESSAGE = "\r\n";

        int fd;
        struct sockaddr * addr;
        socklen_t len;

        SSL_CTX * ssl_ctx;
        SSL * c_ssl;

        std::string sender_mail;

        static bool check_status_code(const char * msg, const char * code) noexcept;
        void initialize_ssl() const;
        void shutdown_ssl() const;

    public:
        SmtpServer(addrinfo * info);
        SmtpServer(SmtpServer & server) = delete;
        SmtpServer(SmtpServer && server) noexcept;
        ~SmtpServer();
        void send_message(const std::string & msg, bool is_split = false, bool is_ssl = false) const;
        std::string receive_message(bool is_ssl = false) const;
        void handshake() const;
        void login(const std::string & username, const std::string & password);
        void tls_handshake();
        void
        mail_send(const std::string & recv_mail_address, const std::string & subject, const std::string & body) const;
        void closed() const;

    };

}
#endif //SMTP_CLIENT_SMTP_SERVER_H
