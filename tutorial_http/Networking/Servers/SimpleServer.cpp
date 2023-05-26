#include "SimpleServer.hpp"

SimpleServer::SimpleServer(int domain, int service, int protocol,
    int port, u_long interface, int bklg)
    {
        socket = new ListenSocket(domain, service, protocol, port,
        interface, bklg);
    }