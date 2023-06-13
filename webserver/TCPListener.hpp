#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class TCPListener
{
    public:

        TCPListener(const char * ipAddress, int port) :
            m_ipAddress(ipAddress),m_port(port) {}
        
        // Initialize the listener
        int init();
        // Run the listener
        int run();

    protected:

    private:

    const char * m_ipAddress; // IP Address server will run on
    int          m_port;      // Port # for the web service
    int          m_socket;    // Internal FD for the listening socket

};