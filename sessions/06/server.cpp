#include <arpa/inet.h>
#include <cstring>
#include <mutex>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "TextBuffer.hpp"

using namespace std;

vector<int> clients;
mutex clients_mutex;

void broadcast(const vector<uint8_t>& msg) {
	lock_guard lock(clients_mutex);
	for (const int fd : clients)
		send(fd, msg.data(), msg.size(), 0);
}

vector<uint8_t> serialize(const Change& c) {
	vector<uint8_t> buf;
	buf.resize(1 + 4 + 4 + 4 + c.payload.size());

	buf[0] = static_cast<uint8_t>(c.type);

	const auto p = reinterpret_cast<uint32_t*>(buf.data() + 1);
	p[0] = htonl(c.line);
	p[1] = htonl(c.col);
	p[2] = htonl(c.payload.size());

	memcpy(buf.data() + 13, c.payload.data(), c.payload.size());
	return buf;
}

void handle_client(const int fd) {
	{
		lock_guard lock(clients_mutex);
		clients.push_back(fd);
	}

	while (true) {
		uint8_t header[13];
		const ssize_t r = recv(fd, header, 13, MSG_WAITALL);
		if (r <= 0)
			break;

		Change c;
		c.type = static_cast<ChangeType>(header[0]);

		const auto p = reinterpret_cast<uint32_t*>(header + 1);
		c.line = ntohl(p[0]);
		c.col = ntohl(p[1]);

		if (const uint32_t len = ntohl(p[2])) {
			c.payload.resize(len);
			recv(fd, c.payload.data(), len, MSG_WAITALL);
		}

		broadcast(serialize(c));
	}

	close(fd);
}

int main() {
	const int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5555);
	addr.sin_addr.s_addr = INADDR_ANY;

	bind(server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	listen(server_fd, 16);

	while (true) {
		int client_fd = accept(server_fd, nullptr, nullptr);
		thread(handle_client, client_fd).detach();
	}
}
