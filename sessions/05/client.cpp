#include <cstring>
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
	addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP (gets acknowledge packets)

	int status;
	if ((status = getaddrinfo(server_name, port, &hints, &res)) != 0) {
		throw runtime_error(string("DNS lookup failed: ") + gai_strerror(status));
	}
	char msg[1024] = "Hello server...";
	const int last_letter = strlen(msg);
	char buf[4096];

	for (int message_num = 0; message_num < 5; message_num++) { // let's write to the server 5 times, just to harass it
		msg[last_letter] = '0' + message_num;
		msg[last_letter+1] = '\0'; // terminate the string with a nul character to make C happy

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

		int bytes_written = send(sock, msg, last_letter+1, 0);
		if (bytes_written == -1) {
			close(sock);
			throw runtime_error("Failed to send message");
		}

		// now send a bigger chunk of data, 1MB?
		constexpr int n = 1024 * 1024;
		const auto p = new char[n];
		for (auto i = 0; i < n; ++i)
			p[i] = 'A';
		int total = 0;
		while (total < n) {
    		int written = send(sock, p + total, n - total, 0);
			if (written == -1) {
				cerr << "lost contact with socket total=" << total << '\n';
			}
    		total += written;
		}
		cout << "Bytes written: " << total << endl;
		// after each message is sent, check for a reply
		if (const int bytes_read = recv(sock, buf, sizeof(buf) - 1, 0); bytes_read > 0) {
			buf[bytes_read] = '\0';
			cout << "Server: " << buf << endl;
		}
		close(sock);
		cout << "Sent message burst: " << message_num << endl;
	}
	freeaddrinfo(res); // give back the memory!

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
