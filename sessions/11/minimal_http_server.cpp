#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>

const char* response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 13\r\n"
    "\r\n"
    "Hello, world!";

void http_server(int port) {
    std::cout << "Starting HTTP server on port " << port << std::endl;
    // Here you would add the code to initialize and run your HTTP server


    // For demonstration purposes, we'll just print a message and exit

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        throw std::runtime_error("Failed to create socket");
    }
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port); //  00 04
    if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(socket_fd);
        throw std::runtime_error("Failed to bind socket");
    }
    if (listen(socket_fd, 10) == -1) {
        close(socket_fd);
        throw std::runtime_error("Failed to listen on socket");
    }
    // this is single threaded. In order to get higher performance,
    // you would need to use multiple threads or processes to handle 
    // multiple connections simultaneously.
    // recommendation: use a thread pool.
    // extra credit: +20 use a thread
    // extra credit +40 use a thread pool.
    // thread pool is hard because it must be thread safe
    // you may use a library or a AI to help.
    std::cout << "HTTP server is running on port " << port << std::endl;
    sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_fd == -1) {
        close(socket_fd);
        throw std::runtime_error("Failed to accept connection");
    }
    char buffer[1024];
    int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        close(client_fd); // this is not production quality!
        close(socket_fd); // should the whole program die if your connection is lost?
        throw std::runtime_error("Failed to receive data");
    }
    std::cout << "Received data: " << buffer << std::endl;
    // TODO: Parse the HTTP request line from buffer: "GET /filename HTTP/1.1"
    // Extract method (GET), path (/filename), and version (HTTP/1.1)
    send(client_fd, response, strlen(response), 0);
    close(client_fd);
    close(socket_fd);
}


int main(int argc, char*argv[]) {
    int port;
    if (argc < 2) {
        port = 8000;
    } else {
        port = atoi(argv[1]);
    }

    try {
        http_server(port);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}