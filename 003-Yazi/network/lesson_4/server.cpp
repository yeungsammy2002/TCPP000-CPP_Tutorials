#include <iostream>

#include <utilities/socket.h>

int main()
{
    auto logger = Singleton<Logger>::instance();
    logger->set_console(true);
    logger->open("./../server.log");

    Socket server;

    server.bind("127.0.0.1", 8080);

    server.listen(1024);

    while (true)
    {
        int connfd = server.accept();
        if (connfd < 0)
        {
            return 1;
        }

        Socket client(connfd);

        char buf[1024] = { 0 };

        size_t len = client.recv(buf, sizeof(buf));
        printf("recv: connfd=%d msg=%s\n", connfd, buf);

        client.send(buf, len);
    }

    server.close();
    return 0;
}
