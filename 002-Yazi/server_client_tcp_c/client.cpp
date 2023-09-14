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
    } else
        printf("socket creating successful\n");

    // connecting to server
    std::string ip = "127.0.0.1";
    int port = 8080;

    struct sockaddr_in sockaddr;
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    // sin_addr is struc in_addr type, in_addr has only one member s_addr,
    // s_addr is unsigned long type,
    // inet_addr() take IP address in cstring "xxx.xxx.xxx.xxx" then return unsigned long
    sockaddr.sin_port = htons(port);
    // sin_port is unsiged short type
    if (::connect(sockfd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) < 0) {
        printf("socket connection error: errno=%d errmsg=%s\n", errno, strerror(errno));
        return 1;
    }

    // sending data to server
    std::string data = "hello world";
    ::send(sockfd, data.c_str(), data.size(), 0);

    // receiving data from server
    char buf[1024] { 0 };
    ::recv(sockfd, buf, sizeof(buf), 0);
    printf("recv: msg=%s", buf);

    // closing socket
    ::close(sockfd);
}
