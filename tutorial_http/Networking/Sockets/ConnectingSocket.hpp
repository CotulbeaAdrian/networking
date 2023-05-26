#ifndef ConnectingSocket_hpp
#define ConnectingSocket_hpp

#include <iostream>

#include "SimpleSocket.hpp"

class ConnectingSocket : public SimpleSocket
{
    public:
        // Constructor
        ConnectingSocket(int  domain, int service, int protocol, int port,
            u_long interface) : SimpleSocket(domain, service, protocol, port,
            interface) {};
        // Virtual function from parent
        int connect_to_network( int sock, struct sockaddr_in address);
};

#endif