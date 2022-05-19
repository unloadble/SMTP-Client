#include "exceptions/connection_error.h"

namespace smtp::exceptions {
    const char * ConnectionError::what() const noexcept {
        return "Failed to connect to the server !";
    }
}
