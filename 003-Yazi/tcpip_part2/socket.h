#pragma once

#include <string>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <utilities/logger.h>

using namespace demo::utilities;

using std::string;

namespace demo {
namespace socket {

class Socket
{
public:
    Socket();

    Socket(int sockfd);

    virtual ~Socket();

    bool bind(const string & ip, const int port);

    bool connect(const string & ip, const int port);

    bool listen(const int backlog);

    int accept();

    size_t send(const char * buff, size_t len);

    size_t recv(char * buff, size_t len);

    void close();

    bool set_non_blocking();

    bool set_send_buffer(int size);

    bool set_recv_buffer(int size);

    bool set_linger(bool active, int seconds);

    bool set_keepalive();

    bool set_reuseaddr();

private:
    string m_ip;
    int m_port;
    int m_sockfd;
};

}
}

