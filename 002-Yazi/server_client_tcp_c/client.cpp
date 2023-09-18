#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // 1. creating socket
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        printf("socket creating error: errno=%d errmsg=%s\n", errno, std::strerror(errno));
        return 1;
    } else
        printf("socket creating successful\n");

    // 2. connecting to server
    std::string ip = "127.0.0.1";
    int port = 8080;

    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);
    if (::connect(sockfd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) < 0) {
        printf("socket connecting error: errno=%d errmsg=%s\n", errno, std::strerror(errno));
        return 1;
    }

    // 3. sending data to server
    std::string data = "hello world";
    ::send(sockfd, data.c_str(), data.size(), 0);

    // 4. receiving data from server
    char buff[1024] { 0 };
    ::recv(sockfd, buff, sizeof(buff), 0);
    printf("recv: msg=%s", buff);

    // 5. closing socket
    ::close(sockfd);
}
