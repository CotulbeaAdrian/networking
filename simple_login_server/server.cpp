#include <iostream>
#include <string>
#include <sstream>
#include <asio.hpp>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cstring> // Include the <cstring> header for strlen

using namespace asio;
using namespace asio::ip;

// Handles an incoming HTTP request
void handleRequest(tcp::socket& socket, const std::string& request, sql::mysql::MySQL_Driver* driver)
{
    std::string response_body;

    // Extract the request method and path
    std::stringstream request_stream(request);
    std::string method, path, http_version;
    request_stream >> method >> path >> http_version;

    // Check the request path and generate the appropriate response
    if (method == "GET" && path == "/greeting")
    {
        response_body = "HTTP/1.1 200 OK\r\nContent-Length: 19\r\n\r\nHello, World!";
    }
    else if (method == "GET" && path == "/date")
    {
        std::time_t now = std::time(nullptr);
        std::string date = std::ctime(&now);
        response_body = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(date.length()) + "\r\n\r\n" + date;
    }
    else if (method == "POST" && path == "/register")
    {
        std::string username, password;
        if (request_stream >> username && request_stream >> password)
        {
            try {
                // Establish a connection to the MySQL database
                sql::Connection* con = driver->connect("tcp://127.0.0.1:3306", "username", "password");
                con->setSchema("database_name");

                // Perform registration logic
                sql::PreparedStatement* stmt = con->prepareStatement("INSERT INTO users (username, password) VALUES (?, ?)");
                stmt->setString(1, username);
                stmt->setString(2, password);
                stmt->executeUpdate();
                delete stmt;

                response_body = "HTTP/1.1 200 OK\r\nContent-Length: 20\r\n\r\nRegistration success";

                delete con;
            } catch (sql::SQLException& e) {
                response_body = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: " + std::to_string(strlen(e.what())) + "\r\n\r\n" + e.what();
            }
        }
        else
        {
            response_body = "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nInvalid data";
        }
    }
    else if (method == "POST" && path == "/login")
    {
        std::string username, password;
        if (request_stream >> username && request_stream >> password)
        {
            try {
                // Establish a connection to the MySQL database
                sql::Connection* con = driver->connect("tcp://127.0.0.1:3306", "username", "password");
                con->setSchema("database_name");

                // Perform login logic
                sql::PreparedStatement* stmt = con->prepareStatement("SELECT * FROM users WHERE username = ? AND password = ?");
                stmt->setString(1, username);
                stmt->setString(2, password);
                sql::ResultSet* res = stmt->executeQuery();

                if (res->next())
                {
                    response_body = "HTTP/1.1 200 OK\r\nContent-Length: 17\r\n\r\nLogin success";
                }
                else
                {
                    response_body = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 16\r\n\r\nLogin failed";
                }

                delete res;
                delete stmt;
                delete con;
            } catch (sql::SQLException& e) {
                response_body = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: " + std::to_string(strlen(e.what())) + "\r\n\r\n" + e.what();
            }
        }
        else
        {
            response_body = "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nInvalid data";
        }
    }
    else
    {
        response_body = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
    }

    // Send HTTP response
    asio::write(socket, asio::buffer(response_body));

    // Close the connection
    socket.shutdown(tcp::socket::shutdown_both);
}

int main()
{
    // Create the I/O context
    asio::io_context io_context;

    // Create the acceptor and bind it to port 8080
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

    // Create MySQL driver
    sql::mysql::MySQL_Driver* driver;
    driver = sql::mysql::get_mysql_driver_instance();

    // Accept and handle incoming connections
    while (true)
    {
        // Create a socket
        tcp::socket socket(io_context);

        // Wait for and accept a connection
        acceptor.accept(socket);

        // Read the request
        asio::streambuf buffer;
        asio::read_until(socket, buffer, "\r\n\r\n");

        // Extract the request string
        std::string request = asio::buffer_cast<const char*>(buffer.data());

        // Handle the request
        handleRequest(socket, request, driver);
    }

    return 0;
}
