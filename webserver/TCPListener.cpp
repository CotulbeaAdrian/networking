#include "TCPListener.hpp"

int TCPListener::init()
{
    // Create a socket
    m_socket = socket (AF_INET, SOCK_STREAM, 0);
    if( m_socket < 0 )
    {
        return -1;
    }

    // Bind the IP address and port to a socket
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(m_port);
    inet_pton(AF_INET, m_ipAddress, &address.sin_addr);

    if (bind(m_socket, (sockaddr*)&address, sizeof(address)) < 0)
    {
        return -1;
    }
}

int TCPListener::run()
{

}