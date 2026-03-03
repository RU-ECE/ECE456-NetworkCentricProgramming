#include "text_buffer.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <ncurses.h>
#include <vector>
#include <cstring>

using namespace std;

int sockfd;
TextBuffer buffer;
Mark cursor;

vector<uint8_t> serialize(const Change& c) {
    vector<uint8_t> buf;
    buf.resize(1 + 4 + 4 + 4 + c.payload.size());

    buf[0] = (uint8_t)c.type;
    uint32_t* p = (uint32_t*)(buf.data() + 1);
    p[0] = htonl(c.line);
    p[1] = htonl(c.col);
    p[2] = htonl(c.payload.size());

    memcpy(buf.data() + 13, c.payload.data(), c.payload.size());
    return buf;
}

void send_change(const Change& c) {
    auto msg = serialize(c);
    send(sockfd, msg.data(), msg.size(), 0);
}

void receiver() {
    while (true) {
        uint8_t header[13];
        if (recv(sockfd, header, 13, MSG_WAITALL) <= 0) break;

        Change c;
        c.type = (ChangeType)header[0];

        uint32_t* p = (uint32_t*)(header + 1);
        c.line = ntohl(p[0]);
        c.col  = ntohl(p[1]);
        uint32_t len = ntohl(p[2]);

        if (len) {
            c.payload.resize(len);
            recv(sockfd, c.payload.data(), len, MSG_WAITALL);
        }

        buffer.apply(c);
    }
}

int main() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(sockfd, (sockaddr*)&addr, sizeof(addr));

    thread(receiver).detach();

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    while (true) {
        erase();
        for (size_t i = 0; i < buffer.lines.size(); ++i)
            mvprintw(i, 0, "%s", buffer.lines[i].c_str());

        move(cursor.line, cursor.col);
        refresh();

        int ch = getch();

        if (isprint(ch)) {
            Change c{ChangeType::InsertChar,
                     (uint32_t)cursor.line,
                     (uint32_t)cursor.col,
                     string(1, (char)ch)};
            send_change(c);
            cursor.col++;
        }
        else if (ch == KEY_BACKSPACE) {
            Change c{ChangeType::DeleteChar,
                     (uint32_t)cursor.line,
                     (uint32_t)cursor.col - 1,
                     ""};
            send_change(c);
            if (cursor.col) cursor.col--;
        }
    }

    endwin();
}