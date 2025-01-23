#include <iostream>

#include <utilities/socket.h>

using std::string;

int main()
{
    auto logger = Singleton<Logger>::instance();
    logger->set_console(true);
    logger->open("./../client.log");

    Socket client;

    client.connect("127.0.0.1", 8080);

    string data = "hello world";
    client.send(data.c_str(), data.size());

    char buf[1024]{ 0 };
    client.recv(buf, sizeof(buf));
    printf("recv: %s\n", buf);

    client.close();

    return 0;
}