#pragma once

#include <socket/socket.h>

namespace demo {
namespace socket {

class ServerSocket : public Socket
{
public:
    ServerSocket() = delete;

    ServerSocket(const string & ip, int port);

    ~ServerSocket() = default;
};
}
}