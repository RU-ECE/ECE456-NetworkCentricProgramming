#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

/*
  RAII:  Resource Acquisition Is Initialization. This is a common
  technique to avoid this problem inC++. Tie any allocation of resources to 
  an object
*/
void connect_to_server(const char* server_name, const char* port) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP (gets acknowledge packets)

    int status = getaddrinfo(server_name, port, &hints, &res);
    if (status != 0) {
        throw runtime_error(string("DNS lookup failed: ") + gai_strerror(status));
    }

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        freeaddrinfo(res);
        throw runtime_error("Could not create socket");
    }

    if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
        close(sock);
        freeaddrinfo(res);
        throw runtime_error("Connection failed");
    }
    freeaddrinfo(res); // give back the memory!

    int bytes_written = send(sock, "Hello, Server!", 14, 0);
    if (bytes_written == -1) {
        close(sock);
        throw runtime_error("Failed to send message");
    }

    const int n = 1024*1024;
    char* p = new char[n];
    for (int i = 0; i < n; ++i) {
        p[i] = 'A';
    }
    bytes_written = send(sock, p, n, 0);
    cout << "Bytes written: " << bytes_written << endl;
    if (bytes_written == -1) {
        close(sock);
        throw runtime_error("Failed to send message");
    }

    char buf[4096];
    int bytes_read = recv(sock, buf, sizeof(buf) - 1, 0);
    if (bytes_read > 0) {
        buf[bytes_read] = '\0';
        cout << "Server: " << buf << endl;
    }

    cout << "Connected to server successfully!" << endl;
    close(sock);
}

int main(int argc, char* argv[]) {
    const char* server_name = argc > 1 ? argv[1] : "localhost";
    const char* port = argc > 2 ? argv[2] : "8080";
    cout << "Connecting to server: " << server_name << " on port: " << port << endl;

    try {
        connect_to_server(server_name, port);
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}