#include <iostream>

#include <socket/Socket.h>
#include <utilities/Logger.h>

using namespace demo::socket;
using namespace demo::utilities;

int main() {
//    std::cout << "server" << std::endl;
    Singleton<Logger>::instance()->open("./test.log");

    //  1. creating socket
    Socket server;

    // 2. binding socket
    if (!server.bind("127.0.0.1", 8080))
        return 1;

    // 3. socket listening
    if (!server.listen(1024))
        return 1;

    while (true) {
        // 4. socket accepting
        int connfd = server.accept();
        if (connfd < 0) return 1;
        Socket client(connfd);
        char buff[1024] { 0 };
        int len = client.recv(buff, sizeof(buff));

        printf("recv: connfd=%d msg=%s\n", connfd, buff);

        client.send(buff, len);
    }
}