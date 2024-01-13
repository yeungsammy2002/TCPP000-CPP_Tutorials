#pragma once

#include <string>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

using std::string;

namespace demo {
namespace socket {

class Socket
{
public:
    Socket();

    Socket(int sockfd);

    ~Socket();

    bool bind(const string & ip, int port);

    bool listen(int backlog);

    bool connect(const string & ip, int port);

    int accept();

    std::size_t recv(char * buff, std::size_t len);

    std::size_t send(const char * buff, std::size_t len);

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