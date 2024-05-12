#include <iostream>

#include <socket/socket.h>

using namespace demo::socket;

int main()
{
//    std::cout << "Tcpip part 2 server demo" << std::endl;

    Socket server;
    server.bind("127.0.0.1", 8080);
    server.listen(1024);
    while (true)
    {
        int connfd = server.accept();
        if (0 > connfd)
        {
            return 1;
        }
        Socket client(connfd);
        char buff[1024]{ 0 };
        auto len = client.recv(buff, 1024);
        std::printf("recv: msg=%s\n", buff);
        client.send(buff, len);
    }
    server.close();

    return 0;
}
