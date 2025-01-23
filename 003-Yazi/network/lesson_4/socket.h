#pragma once

#include <string>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <utilities/logger.h>

using std::string;

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
    int send(const char * buf, int len);
    int recv(char * buf, int len);
    void close();

protected:
    string m_ip;
    int m_port;
    int m_sockfd;
};