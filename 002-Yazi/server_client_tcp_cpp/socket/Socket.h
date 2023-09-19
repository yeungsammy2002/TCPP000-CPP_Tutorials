#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#include <utilities/Logger.h>

namespace demo {
    namespace socket {

        class Socket {
        public:
            Socket();

            Socket(int sockfd);

            ~Socket();

            bool bind(const std::string & ip, int port);

            bool listen(int backlog);

            bool connect(const std::string & ip, int port);

            int accept();

            int send(const char * buff, int len);

            int recv(char * buff, int len);

            void close();

        protected:
            std::string m_ip;
            int m_port;
            int m_sockfd;
        };
    }
}