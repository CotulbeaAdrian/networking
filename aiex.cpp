#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using namespace boost::asio;

// Handles an incoming HTTPS connection
void handleConnection(boost::asio::ssl::stream<ip::tcp::socket>& socket)
{
    // Perform SSL handshake
    socket.handshake(boost::asio::ssl::stream_base::server);
    
    // Read the HTTP request
    boost::asio::streambuf request;
    boost::asio::read_until(socket, request, "\r\n\r\n");
    
    // Print the request
    std::cout << "Received request:\n" << boost::asio::buffer_cast<const char*>(request.data()) << std::endl;
    
    // Send HTTP response
    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    boost::asio::write(socket, boost::asio::buffer(response));
    
    // Shutdown SSL and close the connection
    socket.shutdown();
}

int main()
{
    // Create the I/O context
    boost::asio::io_context io_context;
    
    // Create an SSL context
    boost::asio::ssl::context ssl_context(boost::asio::ssl::context::sslv23);
    
    // Load the server certificate and private key
    ssl_context.use_certificate_chain_file("server.crt");
    ssl_context.use_private_key_file("server.key", boost::asio::ssl::context::pem);
    
    // Create the acceptor and bind it to port 443 (HTTPS)
    boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 443));
    
    // Accept and handle incoming connections
    while (true)
    {
        // Create a socket
        boost::asio::ssl::stream<ip::tcp::socket> socket(io_context, ssl_context);
        
        // Wait for and accept a connection
        acceptor.accept(socket.lowest_layer());
        
        // Handle the connection
        handleConnection(socket);
    }
    
    return 0;
}

