#include"ListenSocket.hpp"

ListenSocket::ListenSocket(int domain, int service, int protocol,
    int port, u_long interface, int bklog) : 
    BindSocket(domain, service, protocol, port, interface)
{
    backlog = bklog;
    start_listening();
    test_connection(listening);
}

void ListenSocket::start_listening()
{
    listening = listen(get_sock(), backlog);
}