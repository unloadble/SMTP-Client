#include "exceptions/closing_error.h"

namespace smtp::exceptions {
    const char * ClosingError::what() const noexcept {
        return "Closing client connection failed !";
    }
}
