cmake_minimum_required(VERSION 3.21)
project(smtp_client)

set(CMAKE_CXX_STANDARD 20)


add_executable(
        smtp_client main.cpp
        src/smtp_server.cpp
        src/exceptions/connection_error.cpp
        src/exceptions/closing_error.cpp
        src/exceptions/sending_error.cpp
        src/exceptions/handshaking_error.cpp
        src/encoders/base64.cpp
        src/exceptions/ssl_error.cpp
        src/exceptions/mail_error.cpp
        src/exceptions/auth_error.cpp
)

# OpenSSL Link
find_package(OpenSSL REQUIRED)
target_link_libraries(smtp_client OpenSSL::SSL)

# Simple Json
find_package(simple-json 0.1.0 REQUIRED)
target_link_libraries(smtp_client simple-json)

target_include_directories(smtp_client PRIVATE include)
