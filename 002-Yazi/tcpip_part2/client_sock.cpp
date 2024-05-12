#include <iostream>

#include <socket/client_socket.h>

using namespace demo::socket;

int main()
{
//    std::cout << "Tcpip part 3 client demo" << std::endl;

    ClientSocket client("127.0.0.1", 8080);
    const string & data = "hello world";
    client.send(data.c_str(), data.size());
    char buff[1024]{ 0 };
    client.recv(buff, sizeof(buff));
    std::printf("recv: msg=%s\n", buff);
    client.close();
}