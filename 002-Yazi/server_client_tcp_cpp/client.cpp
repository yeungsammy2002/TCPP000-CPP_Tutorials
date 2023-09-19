#include <iostream>

#include <socket/socket.h>
#include <string>

using namespace demo::socket;
using namespace demo::utilities;

int main() {
//    std::cout << "client" << std::endl;

    Socket client;

    if (!client.connect("127.0.0.1", 8080))
        return 1;

    std::string data = "hello world";
    client.send(data.c_str(), data.size());

    char buff[1024] { 0 };
    client.recv(buff, sizeof(buff));

    printf("recv: msg=%s", buff);
}