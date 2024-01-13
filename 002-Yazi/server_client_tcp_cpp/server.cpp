#include <iostream>
#include <string>

#include <socket/server_socket.h>

using namespace demo::socket;
using std::string;

int main()
{
    const string ip = "127.0.0.1";
    const int port = 8080;
    const int buff_sz = 1024;

    ServerSocket server(ip, port);

    while (true)
    {
        int connfd = server.accept();

        if (0 > connfd)
            return 1;

        Socket client(connfd);
        char buff[buff_sz]{ 0 };
        std::size_t len = client.recv(buff, buff_sz);
        std::printf("recv: msg=%s\n", buff);
        client.send(buff, len);
    }
}

//#include <iostream>
//
//#include <socket/socket.h>
//
//using namespace demo::socket;
//
//int main()
//{
////    std::cout << "server client socket demo - server" << std::endl;
//
//    const string ip = "127.0.0.1";
//    const int port = 8080;
//    const int backlog = 1024;
//    const int buff_sz = 1024;
//
//    Socket server;
//
//    if (!server.bind(ip, port))
//        return 1;
//
//    if (!server.listen(backlog))
//        return 1;
//
//    while (true)
//    {
//        int connfd = server.accept();
//
//        if (0 > connfd)
//            return 1;
//
//        Socket client(connfd);
//        char buff[buff_sz]{ 0 };
//        std::size_t len = client.recv(buff, buff_sz);
//        std::printf("recv: msg=%s\n", buff);
//        client.send(buff, len);
//    }
//}