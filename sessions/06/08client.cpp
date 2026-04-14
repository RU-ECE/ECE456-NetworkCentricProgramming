#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

/*
  RAII:  Resource Acquisition Is Initialization. This is a common
  technique to avoid this problem inC++. Tie any allocation of resources to
  an object
*/
void connect_to_server(const char* server_name, const char* port) {
	addrinfo hints = {}, *res;
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP (gets acknowledge packets)

	if (const int status = getaddrinfo(server_name, port, &hints, &res); status != 0)
		throw runtime_error(string("DNS lookup failed: ") + gai_strerror(status));

	const int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
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

	char buf[4096]; // buffer has limited size!!!

	for (int num_messages = 0; num_messages < 100; num_messages++) {
		if (const int bytes_written = send(sock, "Hello, Server!", 14, 0); bytes_written == -1) {
			close(sock);
			throw runtime_error("Failed to send message");
		}

		// this read will not allow buffer overflow. Not the greatest, most secure test though
		if (const int bytes_read = recv(sock, buf, sizeof(buf) - 1, 0); bytes_read > 0) {
			buf[bytes_read] = '\0';
			cout << "Server: " << buf << endl;
		}
	}

	cout << "Connected to server successfully!" << endl;
	close(sock);
}

int main(const int argc, char* argv[]) {
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
