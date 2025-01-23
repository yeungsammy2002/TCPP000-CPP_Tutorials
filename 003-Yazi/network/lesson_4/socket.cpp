#include <utilities/socket.h>





Socket::Socket() : m_ip(""), m_port(0), m_sockfd(0)
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd < 0)
    {
        log_error("create socket error: errno=%d errmsg=%s", errno, strerror(errno));
    }
    log_debug("create socket success!");
}

Socket::Socket(int sockfd) : m_ip(""), m_port(0), m_sockfd(sockfd)
{
}

Socket::~Socket()
{
    close();
}





bool Socket::bind(const string & ip, int port)
{
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    if (ip.empty())
    {
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else
    {
        sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    sockaddr.sin_port = htons(port);
    if (::bind(m_sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0)
    {
        log_error("socket bind error: errno=%d errmsg=%s", errno, strerror(errno));
        return false;
    }

    m_ip = ip;
    m_port = port;
    log_debug("socket bind success: ip=%s port=%d", ip.c_str(), port);
    return true;
}

bool Socket::listen(int backlog)
{
    if (::listen(m_sockfd, backlog) < 0)
    {
        log_error("socket listen error: errno=%d errmsg=%s", errno, strerror(errno));
        return false;
    }
    log_debug("socket listening...");
    return true;
}

bool Socket::connect(const string & ip, int port)
{
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);
    if (::connect(m_sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0)
    {
        log_error("socket connect error: errno=%d errmsg=%s", errno, strerror(errno));
        return false;
    }
    m_ip = ip;
    m_port = port;
    return true;
}

int Socket::accept()
{
    int connfd = ::accept(m_sockfd, nullptr, nullptr);
    if (connfd < 0)
    {
        log_error("socket accept error: errno=%d errmsg=%s", errno, strerror(errno));
        return connfd;
    }
    log_debug("socket accept: conn=%d", connfd);
    return connfd;
}

int Socket::send(const char * buf, int len)
{
    return ::send(m_sockfd, buf, len, 0);
}

int Socket::recv(char * buf, int len)
{
    return ::recv(m_sockfd, buf, len, 0);
}

void Socket::close()
{
    if (m_sockfd > 0)
    {
        ::close(m_sockfd);
        m_sockfd = 0;
    }
}
