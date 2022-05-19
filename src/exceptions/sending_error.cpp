#include "exceptions/sending_error.h"

namespace smtp::exceptions {
    const char * SendingError::what() const noexcept {
        return "Failed to send packets to server !";
    }
}
