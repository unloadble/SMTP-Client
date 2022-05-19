
#ifndef SMTP_CLIENT_BASE64_H
#define SMTP_CLIENT_BASE64_H

#include <string>

namespace smtp::encoder {
    const std::string BASE64_CHARS {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

    class Base64 {
    private:
        static inline bool is_base64(unsigned char c);
    public:
        static std::string base64_encode(unsigned char const * bytes_to_encode, size_t in_len);
        static std::string base64_decode(std::string const & encoded_string);
    };
}

#endif //SMTP_CLIENT_BASE64_H
