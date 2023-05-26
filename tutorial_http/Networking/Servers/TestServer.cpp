#include "TestServer.hpp"
#include <unistd.h>
#include <cstring>

TestServer::TestServer() : SimpleServer(AF_INET, SOCK_STREAM, 0,
    80, INADDR_ANY, 10)
{
    launch();
};

void TestServer::accepter()
{
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr*)&address, (socklen_t*)&addrlen);

    read(new_socket, buffer, sizeof(buffer));
}

void TestServer::handler()
{
    std::cout << buffer << std::endl;
}

void TestServer::responder()
{
    char *hello = "Hello from server";
    write(new_socket, hello, strlen(hello));
    close(new_socket);
}

void TestServer::launch()
{
    while(true)
    {
        std::cout << "=========== Waiting ===========" << std::endl;
        accepter();
        handler();
        responder();
        std::cout << "=========== DONE ===========" << std::endl;
    }
}