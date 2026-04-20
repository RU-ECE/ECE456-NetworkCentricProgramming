#include <arpa/inet.h>
#include <array>
#include <atomic>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <mutex>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "NcursesCompat.hpp"
#include "TextBuffer.hpp"

using namespace std;

namespace {

	constexpr uint16_t kPort = 5555;
	constexpr char kHost[] = "127.0.0.1";
	constexpr size_t kHeaderSize = 13;

	int sockfd = -1;
	atomic<bool> running{true};
	mutex state_mutex;
	TextBuffer buffer;
	Mark cursor;

	void write_u32(uint8_t* dest, const uint32_t value) {
		const uint32_t network_value = htonl(value);
		memcpy(dest, &network_value, sizeof(network_value));
	}

	uint32_t read_u32(const uint8_t* src) {
		uint32_t network_value = 0;
		memcpy(&network_value, src, sizeof(network_value));
		return ntohl(network_value);
	}

	bool send_all(const uint8_t* data, const size_t size) {
		size_t total = 0;
		while (total < size) {
			const ssize_t sent = send(sockfd, data + total, size - total, 0);
			if (sent <= 0)
				return false;
			total += static_cast<size_t>(sent);
		}
		return true;
	}

	bool recv_all(uint8_t* data, const size_t size) {
		size_t total = 0;
		while (total < size) {
			const ssize_t received = recv(sockfd, data + total, size - total, 0);
			if (received <= 0)
				return false;
			total += static_cast<size_t>(received);
		}
		return true;
	}

	vector<uint8_t> serialize(const Change& c) {
		vector<uint8_t> buf(kHeaderSize + c.payload.size());

		buf[0] = static_cast<uint8_t>(c.type);
		write_u32(buf.data() + 1, c.line);
		write_u32(buf.data() + 5, c.col);
		write_u32(buf.data() + 9, static_cast<uint32_t>(c.payload.size()));

		if (!c.payload.empty())
			memcpy(buf.data() + kHeaderSize, c.payload.data(), c.payload.size());

		return buf;
	}

	bool send_change(const Change& c) {
		const auto msg = serialize(c);
		if (send_all(msg.data(), msg.size()))
			return true;

		running = false;
		return false;
	}

	void clamp_cursor() {
		if (buffer.lines.empty()) {
			buffer.lines.push_back("");
			cursor = {};
			return;
		}

		if (cursor.line >= buffer.lines.size())
			cursor.line = buffer.lines.size() - 1;
		if (cursor.col > buffer.lines[cursor.line].size())
			cursor.col = buffer.lines[cursor.line].size();
	}

	void receiver() {
		while (running) {
			array<uint8_t, kHeaderSize> header{};
			if (!recv_all(header.data(), header.size()))
				break;

			Change c;
			c.type = static_cast<ChangeType>(header[0]);
			c.line = read_u32(header.data() + 1);
			c.col = read_u32(header.data() + 5);
			const uint32_t len = read_u32(header.data() + 9);

			if (len != 0) {
				c.payload.resize(len);
				if (!recv_all(reinterpret_cast<uint8_t*>(c.payload.data()), len))
					break;
			}

			lock_guard<mutex> lock(state_mutex);
			buffer.apply(c);
			clamp_cursor();
		}

		running = false;
	}

	bool handle_insert(const int ch) {
		Mark current;
		{
			lock_guard<mutex> lock(state_mutex);
			current = cursor;
		}

		if (!send_change(Change{ChangeType::InsertChar, static_cast<uint32_t>(current.line),
								static_cast<uint32_t>(current.col), string(1, static_cast<char>(ch))})) {
			return false;
		}

		lock_guard<mutex> lock(state_mutex);
		++cursor.col;
		return true;
	}

	bool handle_backspace() {
		Mark current;
		{
			lock_guard<mutex> lock(state_mutex);
			if (cursor.col == 0)
				return true;
			current = cursor;
		}

		if (!send_change(Change{ChangeType::DeleteChar, static_cast<uint32_t>(current.line),
								static_cast<uint32_t>(current.col - 1), ""})) {
			return false;
		}

		lock_guard<mutex> lock(state_mutex);
		--cursor.col;
		return true;
	}

	bool handle_newline() {
		Mark current;
		{
			lock_guard<mutex> lock(state_mutex);
			current = cursor;
		}

		if (!send_change(Change{ChangeType::InsertNewline, static_cast<uint32_t>(current.line),
								static_cast<uint32_t>(current.col), ""})) {
			return false;
		}

		lock_guard<mutex> lock(state_mutex);
		++cursor.line;
		cursor.col = 0;
		return true;
	}

	void move_left() {
		lock_guard<mutex> lock(state_mutex);
		if (cursor.col > 0) {
			--cursor.col;
		} else if (cursor.line > 0) {
			--cursor.line;
			cursor.col = buffer.lines[cursor.line].size();
		}
	}

	void move_right() {
		lock_guard<mutex> lock(state_mutex);
		const size_t line_length = buffer.lines[cursor.line].size();
		if (cursor.col < line_length) {
			++cursor.col;
		} else if (cursor.line + 1 < buffer.lines.size()) {
			++cursor.line;
			cursor.col = 0;
		}
	}

	void move_up() {
		lock_guard<mutex> lock(state_mutex);
		if (cursor.line > 0) {
			--cursor.line;
			clamp_cursor();
		}
	}

	void move_down() {
		lock_guard<mutex> lock(state_mutex);
		if (cursor.line + 1 < buffer.lines.size()) {
			++cursor.line;
			clamp_cursor();
		}
	}

	void draw_screen() {
		vector<string> lines;
		Mark cursor_snapshot;
		{
			lock_guard<mutex> lock(state_mutex);
			lines = buffer.lines;
			cursor_snapshot = cursor;
		}

		erase();

		int rows = 0;
		int cols = 0;
		getmaxyx(stdscr, rows, cols);

		const int max_rows = max(rows, 0);
		const int max_cols = max(cols, 0);

		for (int row = 0; row < max_rows && row < static_cast<int>(lines.size()); ++row)
			mvaddnstr(row, 0, lines[row].c_str(), max_cols);

		if (max_rows > 0 && max_cols > 0) {
			const int cursor_y = min<int>(cursor_snapshot.line, max_rows - 1);
			const int cursor_x = min<int>(cursor_snapshot.col, max_cols - 1);
			move(cursor_y, cursor_x);
		}

		refresh();
	}

} // namespace

int main() {
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(kPort);
	if (inet_pton(AF_INET, kHost, &addr.sin_addr) != 1) {
		cerr << "Invalid server address: " << kHost << endl;
		close(sockfd);
		return 1;
	}

	if (connect(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
		perror("connect");
		close(sockfd);
		return 1;
	}

	thread receiver_thread(receiver);

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	timeout(100);

	while (running) {
		draw_screen();

		const int ch = getch();
		if (ch == ERR)
			continue;
		if (ch == 17) // Ctrl-Q
			break;

		if (ch == KEY_LEFT) {
			move_left();
		} else if (ch == KEY_RIGHT) {
			move_right();
		} else if (ch == KEY_UP) {
			move_up();
		} else if (ch == KEY_DOWN) {
			move_down();
		} else if (ch == '\n' || ch == KEY_ENTER) {
			if (!handle_newline())
				break;
		} else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
			if (!handle_backspace())
				break;
		} else if (ch >= 0 && ch <= 255 && isprint(static_cast<unsigned char>(ch))) {
			if (!handle_insert(ch))
				break;
		}
	}

	running = false;
	shutdown(sockfd, SHUT_RDWR);
	close(sockfd);

	if (receiver_thread.joinable())
		receiver_thread.join();

	endwin();
	return 0;
}
