#include <iostream>
#include <netdb.h>
#include "arpa/inet.h"
#include "simple_json.h"
#include "smtp_server.h"

using namespace smtp;
using namespace simple_json;
using namespace ::types;
using namespace ::deserializer;

int main() {
    Json config (load("../config.json"));
    struct addrinfo hints {AI_PASSIVE, AF_UNSPEC, SOCK_STREAM};
    struct addrinfo * smtp_server_info;
    int status;
    if ((status = getaddrinfo(
            config["SmtpAddress"].get_string().c_str(),
            config["Port"].get_string().c_str(),
            & hints,
            & smtp_server_info
    )) != 0) {
        std::cout << "DNS resolve failed : " << gai_strerror(status) << std::endl;
        return 1;
    }
    SmtpServer * connection_server {new SmtpServer {smtp_server_info}};
    try {
        connection_server->handshake();
        connection_server->tls_handshake();
        connection_server->login(config["UserName"].get_string(), config["Password"].get_string());
        connection_server->mail_send("patronplatform@gmail.com", "Test", "Mail Send.");
        connection_server->closed();
    } catch (SmtpError & e) {
        std::cout << e.what() << std::endl;
    }
    freeaddrinfo(smtp_server_info);
    delete connection_server;
    return 0;
}
