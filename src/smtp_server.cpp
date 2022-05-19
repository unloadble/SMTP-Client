#include "smtp_server.h"

namespace smtp {
    SmtpServer::SmtpServer(addrinfo * info) : fd {socket(info->ai_family, info->ai_socktype, info->ai_protocol)},
                                              addr {new sockaddr {*(info->ai_addr)}},
                                              len {info->ai_addrlen} {
        initialize_ssl();
        ssl_ctx = SSL_CTX_new(TLS_client_method());
        c_ssl = SSL_new(ssl_ctx);
        SSL_set_fd(c_ssl, fd);
        if (connect(fd, addr, len) == -1) {
            std::cerr << strerror(errno) << std::endl;
            throw ConnectionError {};
        }
        std::cout << "connected." << std::endl;
    }

    SmtpServer::SmtpServer(SmtpServer && server) noexcept: fd {server.fd}, addr {server.addr}, len {server.len} {
        server.addr = nullptr;
    }

    SmtpServer::~SmtpServer() {
        delete addr;
    }

    void SmtpServer::closed() const {
        send_message("QUIT", false, true);
        if (check_status_code(receive_message(true).c_str(), status_codes::CLOSED))
            std::cout << "Connection Closed." << std::endl;
        else
            std::cout << "Cannot Closed Connection." << std::endl;
        shutdown_ssl();
        if (close(fd) == -1)
            throw ClosingError {};
    }

    void SmtpServer::send_message(const std::string & msg, bool is_split, bool is_ssl) const {

        ssize_t bytes_sent {0}; // TODO
        ssize_t sent;
        std::string temp_msg {msg + END_FRAMING_MESSAGE};
        do {
            const std::string buf {is_split ? temp_msg.substr(bytes_sent) : temp_msg};
            if ((sent = is_ssl ? SSL_write(c_ssl, buf.c_str(), buf.size()) : send(fd, buf.c_str(), buf.size(), 0)) ==
                -1) {
                throw SendingError {};
            }
            bytes_sent += sent;
        } while (bytes_sent != temp_msg.size() && is_split);
    }

    std::string SmtpServer::receive_message(bool is_ssl) const {
        char tmp_buf[1024] {};
        std::string buf;
        ssize_t recv_size;
        while ((recv_size = is_ssl ? SSL_read(c_ssl, tmp_buf, 1024) : recv(fd, tmp_buf, 1024, 0)) > 0) {
            buf.append(tmp_buf, recv_size);
            if (buf.compare(
                    buf.length() - END_FRAMING_MESSAGE.length(), END_FRAMING_MESSAGE.length(), END_FRAMING_MESSAGE
            ) == 0)
                break;
        }
        return buf;
    }

    void SmtpServer::handshake() const {
        if (!check_status_code(receive_message().c_str(), status_codes::SERVICE_READY))
            throw HandshakingError {};
        send_message("EHLO Alice");
        if (!check_status_code(receive_message().c_str(), status_codes::ACTION_COMPLETE))
            throw HandshakingError {};
        std::cout << "Handshake Completed." << std::endl;
    }

    void SmtpServer::login(const std::string & username, const std::string & password) {
        sender_mail = std::string {username};
        send_message("AUTH LOGIN", false, true);
        if (!check_status_code(receive_message(true).c_str(), status_codes::AUTH))
            throw AuthError {AuthErrorType::AUTH_COMMAND};
        send_message(
                Base64::base64_encode(reinterpret_cast<const unsigned char *>(username.c_str()), username.length()),
                false, true);
        if (!check_status_code(receive_message(true).c_str(), status_codes::AUTH))
            throw AuthError {AuthErrorType::INVALID_USERNAME};
        send_message(
                Base64::base64_encode(reinterpret_cast<const unsigned char *>(password.c_str()), password.length()),
                false, true);
        if (!check_status_code(receive_message(true).c_str(), status_codes::ACCEPT_AUTH))
            throw AuthError {AuthErrorType::INVALID_AUTHENTICATION};
        std::cout << "Login Completed." << std::endl;
    }

    bool SmtpServer::check_status_code(const char * msg, const char * code) noexcept {
        return strncmp(msg, code, strlen(code)) == 0;
    }

    void SmtpServer::mail_send(
            const std::string & recv_mail_address, const std::string & subject, const std::string & body
    ) const {

        send_message("MAIL FROM:<" + sender_mail + ">", false, true);
        if (!check_status_code(receive_message(true).c_str(), status_codes::ACTION_COMPLETE))
            throw MailError {MailErrorType::INVALID_FROM_EMAIL};
        send_message("RCPT TO:<" + recv_mail_address + ">", false, true);
        if (!check_status_code(receive_message(true).c_str(), status_codes::ACTION_COMPLETE))
            throw MailError {MailErrorType::INVALID_RCPT_EMAIL};
        std::cout << "Set Sender And Receiver." << std::endl;
        send_message("DATA", false, true);
        if (!check_status_code(receive_message(true).c_str(), status_codes::START_MAIL_INPUT))
            throw MailError {MailErrorType::FAILED_DATA_COMMAND};

        std::stringstream mail_body;
        mail_body << "Mail From: John Q. Public <" << sender_mail << ">\n"
                  << "Subject:" << subject << "\n"
                  << "To:" << recv_mail_address << "\n\n"
                  << body;

        send_message(mail_body.str(), false, true);
        send_message(".", false, true);
        if (!check_status_code(receive_message(true).c_str(), status_codes::ACTION_COMPLETE))
            throw MailError {MailErrorType::INVALID_BODY};
        std::cout << "Mail Send." << std::endl;
    }

    void SmtpServer::initialize_ssl() const {
        SSL_load_error_strings();
        SSL_library_init();
        OpenSSL_add_all_algorithms();
    }

    void SmtpServer::shutdown_ssl() const {
        SSL_shutdown(c_ssl);
        SSL_free(c_ssl);
        SSL_CTX_free(ssl_ctx);
    }

    void SmtpServer::tls_handshake() {
        send_message("STARTTLS");
        if (!check_status_code(receive_message().c_str(), status_codes::SERVICE_READY))
            throw SmtpError {};
        if (SSL_connect(c_ssl) != 1) {
            ERR_print_errors_fp(stderr);
            throw SslError {};
        }
        std::cout << "SSL Handshake Completed." << std::endl;
    }
}