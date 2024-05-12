#include <socket/socket.h>

using namespace demo::socket;

Socket::Socket() : m_ip(""), m_port(0), m_sockfd(0)
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (0 > m_sockfd)
    {
        std::printf("socket create error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket create error: errno=%d, errmsg=%s", errno, std::strerror(errno));
    } else
    {
        std::printf("socket create success!\n");
        log_debug("socket create success!");
    }
}

Socket::Socket(int sockfd) : m_ip(""), m_port(0), m_sockfd(sockfd) {}

Socket::~Socket()
{
    close();
}

bool Socket::bind(const string & ip, const int port)
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

    if (0 > ::bind(m_sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)))
    {
        std::printf("socket bind error: errno=%d errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket bind error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }

    std::printf("socket bind success: ip=%s, port=%d\n", ip.c_str(), port);
    log_debug("socket bind success: ip=%s, port=%d", ip.c_str(), port);

    m_ip = ip;
    m_port = port;
    return true;
}

bool Socket::connect(const string & ip, const int port)
{
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);

    if (0 > ::connect(m_sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)))
    {
        std::printf("socket connect error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket connect error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }

    std::printf("socket connect success: ip=%s, port=%d\n", ip.c_str(), port);
    log_debug("socket connect success: ip=%s, port=%d", ip.c_str(), port);
    m_ip = ip;
    m_port = port;
    return true;
}

bool Socket::listen(const int backlog)
{
    if (0 > ::listen(m_sockfd, backlog))
    {
        std::printf("socket listen error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket listen error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    std::printf("socket listening...\n");
    log_debug("socket listening...");
    return true;
}

int Socket::accept()
{
    int connfd = ::accept(m_sockfd, nullptr, nullptr);
    if (0 > connfd)
    {
        std::printf("socket accept error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket accept error: errno=%d, errmsg=%s", errno, std::strerror(errno));
    } else
    {
        std::printf("socket accept: connfd=%d\n", connfd);
        log_debug("socket accept: connfd=%d", connfd);
    }
    return connfd;
}

size_t Socket::send(const char * buff, size_t len)
{
    return ::send(m_sockfd, buff, len, 0);
}

size_t Socket::recv(char * buff, size_t len)
{
    return ::recv(m_sockfd, buff, len, 0);
}

void Socket::close()
{
    if (0 < m_sockfd)
    {
        ::close(m_sockfd);
        m_sockfd = 0;
    }
}

bool Socket::set_non_blocking()
{
    int flags = fcntl(m_sockfd, F_GETFL, 0);
    if (0 > flags)
    {
        std::printf("socket set_non_blocking error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket set_non_blocking error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    flags |= O_NONBLOCK;
    if (0 > fcntl(m_sockfd, F_SETFL, flags))
    {
        std::printf("socket set_non_blocking error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket set_non_blocking error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_send_buffer(int size)
{
    int buff_size = size;
    if (0 > setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, &buff_size, sizeof(buff_size)))
    {
        std::printf("socket set_send_buffer error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket set_send_buffer error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_recv_buffer(int size)
{
    int buff_size = size;
    if (0 > setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, &buff_size, sizeof(buff_size)))
    {
        std::printf("socket set_recv_buffer error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket set_recv_buffer error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_linger(bool active, int seconds)
{
    struct linger l;
    std::memset(&l, 0, sizeof(l));

    l.l_onoff = active ? 1 : 0;
    l.l_linger = seconds;

    if (0 > setsockopt(m_sockfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l)))
    {
        std::printf("socket set_linger error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket set_linger error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_keepalive()
{
    int flag = 1;
    if (0 > setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)))
    {
        std::printf("socket set_linger error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket set_linger error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_reuseaddr()
{
    int flag = 1;
    if (0 > setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)))
    {
        std::printf("socket set_reuseaddr error: errno=%d, errmsg=%s\n", errno, std::strerror(errno));
        log_error("socket set_reuseaddr error: errno=%d, errmsg=%s", errno, std::strerror(errno));
        return false;
    }
    return true;
}

