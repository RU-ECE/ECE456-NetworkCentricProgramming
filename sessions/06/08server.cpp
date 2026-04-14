#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

void wait_for_client(const char* port) {
	const int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == -1)
		throw runtime_error("Could not create socket");

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET; // IPv4 (TCP/IP) 192.168.192.1
	// ipv6 16 bytes 2001:0db8:85a3:0000:0000:8a2e:0370:7334
	server_addr.sin_port = htons(atoi(port));
	server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

	if (bind(server_sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
		close(server_sock);
		throw runtime_error("Bind failed");
	}
	if (listen(server_sock, 1) < 0) {
		close(server_sock);
		throw runtime_error("Listen failed");
	}

	cout << "Waiting for client connection..." << endl;
	do {
		// thread t1(execute your connection stuff here...)
		// THREADING GOES HERE
		const int client_sock = accept(server_sock, nullptr, nullptr);
		if (client_sock < 0) {
			close(server_sock);
			throw runtime_error("Accept failed");
		}
		if (const int bytes_written = send(client_sock, "Hello, yourself!", 17, 0); bytes_written == -1) {
			close(client_sock);
			throw runtime_error("Failed to send message");
		}
	} while (true);

	cout << "Connected to server successfully!" << endl;
	close(server_sock);
}

int main(const int argc, char* argv[]) {
	// I am the server, I don't have to specify a name. I do have to specify a port to listen on. I can use the default
	// port 8080
	const char* port = argc > 2 ? argv[2] : "8080";
	cout << "Listening on port: " << port << endl;

	try {
		wait_for_client(port);
	} catch (const exception& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	return 0;
}
