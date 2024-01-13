#include <socket/client_socket.h>

using namespace demo::socket;

ClientSocket::ClientSocket(const string & ip, int port) : Socket()
{
    connect(ip, port);
}