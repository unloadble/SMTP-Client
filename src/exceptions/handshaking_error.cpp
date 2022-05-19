#include "exceptions/handshaking_error.h"

namespace smtp::exceptions {
    const char * HandshakingError::what() const noexcept {
        return "handshaking failed !?";
    }
}
