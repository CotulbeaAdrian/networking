#ifndef SimpleSocket_hpp
#define SimpleSocket_hpp

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class SimpleSocket{
    private:
        int sock;
        int connection;
        struct sockaddr_in address;

    public:
        SimpleSocket(int domain, int service, int protocol,
            int port, u_long interface);
        virtual int connect_to_network(int sock,struct sockaddr_in address) = 
            0;
        void test_connection(int);

        int get_sock();
        int get_connection();
        struct sockaddr_in get_address();

};

#endif