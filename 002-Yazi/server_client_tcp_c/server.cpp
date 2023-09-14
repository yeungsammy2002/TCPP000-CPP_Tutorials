#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // creating socket
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        printf("socket creating error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return 1;
    } else {
        printf("socket creating successful\n");
    }

    // binding socket
    std::string ip = "127.0.0.1";
    int port = 8080;

    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);
    if (::bind(sockfd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) < 0) {
        printf("socket binding error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return 1;
    } else
        printf("socket binding successful ip=%s port=%d\n", ip.c_str(), port);

    // socket listening
    if (::listen(sockfd, 1024) < 0) {
        printf("socket listening error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return 1;
    } else
        printf("socket listening ...\n");

    while (true) {
        // accepting client connection
        int connfd = ::accept(sockfd, nullptr, nullptr);
        if (connfd < 0) {
            printf("socket accepting error: errno=%d errmsg=%s\n", errno, strerror(errno));
            return 1;
        }

        char buf[1024] { 0 };

        // receiving client data
        std::size_t len = ::recv(connfd, buf, sizeof(buf), 0);
        printf("recv: connfd=%d msg=%s\n", connfd, buf);

        // sending same data back to client
        ::send(connfd, buf, len, 0);
    }

    ::close(sockfd);
}