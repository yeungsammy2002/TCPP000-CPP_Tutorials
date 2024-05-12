#include <iostream>

#include <socket/server_socket.h>

using namespace demo::socket;

int main()
{
//    std::cout << "Tcpip part 3 server demo" << std::endl;

    ServerSocket server("127.0.0.1", 8080);

    while (true)
    {
        int connfd = server.accept();
        if (0 > connfd)
        {
            return 1;
        }
        Socket client(connfd);
        char buff[1024]{ 0 };
        size_t len = client.recv(buff, sizeof(buff));
        std::printf("recv: msg=%s\n", buff);
        client.send(buff, len);
    }
    server.close();
    return 0;
}