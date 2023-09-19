#include <socket/socket.h>

using namespace demo::socket;
using namespace demo::utilities;

Socket::Socket() : m_ip(""), m_port(0), m_sockfd(0) {
    m_sockfd = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0) {
        log_error("socket creating error: errno=%d errmsg=%s", errno, std::strerror(errno));
    }
    log_debug("socket creating successful");
}

Socket::Socket(int sockfd) : m_ip(""), m_port(0), m_sockfd(sockfd) {}

Socket::~Socket() {
    close();
}

bool Socket::bind(const std::string & ip, int port) {
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;

    if (ip.empty())
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());

    sockaddr.sin_port = (htons(port));
    if (::bind(m_sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
        log_error("socket binding error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    m_ip = ip;
    m_port = port;
    log_debug("socket binding successful ip=%s port=%d", ip.c_str(), port);
    return true;
}

bool Socket::listen(int backlog) {
    if (::listen(m_sockfd, backlog) < 0) {
        log_error("socket listening error: errno=%d errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    log_debug("socket listening...");
    return true;
}

bool Socket::connect(const std::string & ip, int port) {
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);
    if (::connect(m_sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
        log_error("socket connecting error: errno=%d errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    m_ip = ip;
    m_port = port;
    return true;
}

int Socket::accept() {
    int connfd = ::accept(m_sockfd, nullptr, nullptr);
    if (connfd < 0) {
        log_error("socket accepting error: errno=%d errmsg=%s", errno, std::strerror(errno));
        return -1;
    }
    log_debug("socket accepting successful: connfd=%d", connfd);
    return connfd;
}

int Socket::send(const char * buff, int len) {
    return ::send(m_sockfd, buff, len, 0);
}

int Socket::recv(char * buff, int len) {
    return ::recv(m_sockfd, buff, len, 0);
}

void Socket::close() {
    if (m_sockfd > 0) {
        ::close(m_sockfd);
        m_sockfd = 0;
    }
}