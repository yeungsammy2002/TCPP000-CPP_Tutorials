#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::string;

int main()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("create socket error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return 1;
    } else
    {
        printf("create socket success!\n");
    }

    string ip = "127.0.0.1";
    int port = 8080;
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);
    if (::connect(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0)
    {
        printf("socket connect error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return 1;
    }

    string data = "hello world";
    ::send(sockfd, data.c_str(), data.size(), 0);

    char buf[1024]{ 0 };
    ::recv(sockfd, buf, sizeof(buf), 0);
    printf("recv: %s\n", buf);

    ::close(sockfd);

    return 0;
}

// find PID of server.exe(Windows) `netstat -ano|findstr :8080`
// find PID of server.exe(Linux) `netstat -ano|grep :8080`
