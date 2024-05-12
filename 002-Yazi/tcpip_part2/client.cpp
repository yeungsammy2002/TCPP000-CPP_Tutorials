#include <iostream>
#include <string>

#include <socket/socket.h>

using namespace demo::socket;

using std::string;

int main()
{
//    std::cout << "tcpip part 2 client demo" << std::endl;

    Socket client;

    client.connect("127.0.0.1", 8080);

    const string & data = "hello world";
    client.send(data.c_str(), data.size());

    char buff[1024]{ 0 };
    client.recv(buff, 1024);
    std::printf("recv: msg=%s\n", buff);

    return 0;
}