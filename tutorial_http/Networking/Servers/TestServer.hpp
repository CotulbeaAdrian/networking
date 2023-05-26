#ifndef TestServer_hpp
#define TestServer_hpp

#include <iostream>

#include "SimpleServer.hpp"

class TestServer : public SimpleServer
{
    private:
        char buffer[30000] = {0};
        int new_socket;

        void accepter();
        void handler();
        void responder();
    public:
        TestServer();
        void launch();

};

#endif