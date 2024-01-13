#include <iostream>
#include <string>

#include <socket/client_socket.h>

using namespace demo::socket;
using std::string;

int main()
{
    const string & ip = "127.0.0.1";
    const int port = 8080;
    string data = "hello world";
    const int buff_sz = 1024;
    char buff[buff_sz]{ 0 };

    ClientSocket client(ip, port);

    client.send(data.c_str(), data.size());

    client.recv(buff, buff_sz);
    std::printf("recv: msg=%s\n", buff);
}

//#include <iostream>
//
//#include <socket/socket.h>
//
//using namespace demo::socket;
//
//int main()
//{
////    std::cout << "server client socket demo - client" << std::endl;
//
//    const string ip = "127.0.0.1";
//    const int port = 8080;
//    const int buff_sz = 1024;
//    string data = "hello world";
//
//    Socket client;
//    client.connect(ip, port);
//
//    client.send(data.c_str(), data.size());
//    char buff[buff_sz]{ 0 };
//    client.recv(buff, buff_sz);
//    std::printf("recv: msg=%s\n", buff);
//}