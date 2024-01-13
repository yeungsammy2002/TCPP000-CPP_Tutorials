#include <socket/server_socket.h>

using namespace demo::socket;

ServerSocket::ServerSocket(const string & ip, int port) : Socket()
{
//    set_non_blocking();
    set_send_buffer(10 * 1024);
    set_recv_buffer(10 * 1024);
    set_linger(true, 1);
    set_keepalive();
    set_reuseaddr();
    bind(ip, port);
    listen(1024);
}