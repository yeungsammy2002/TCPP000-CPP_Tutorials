#pragma once

#include <string>

#include <socket/socket.h>

using std::string;

namespace demo {
namespace socket {

class ClientSocket : public Socket
{
public:
    ClientSocket() = delete;

    ClientSocket(const string & ip, int port);

    ~ClientSocket() = default;
};
}
}