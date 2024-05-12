#pragma once

#include <socket/socket.h>

namespace demo {
namespace socket {

class ClientSocket : public Socket
{
public:
    ClientSocket() = delete;

    ClientSocket(const string & ip, const int port);

    ~ClientSocket() = default;
};

}
}