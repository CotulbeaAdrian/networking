#ifndef ListenSocket_hpp
#define ListenSocket_hpp

#include <iostream>

#include "BindSocket.hpp"

class ListenSocket : public BindSocket
{
    private:
        int backlog;
        int listening;
    public:
    ListenSocket(int domain, int service, int protocol, int port,
        u_long interface, int bklog);

    void start_listening();
};

#endif