#include "exceptions/ssl_error.h"

namespace smtp::exceptions {
    const char * SslError::what() const noexcept {
        return "SSL Handshake Failed !?";
    }
}
