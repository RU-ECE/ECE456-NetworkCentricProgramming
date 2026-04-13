#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>


// open this file, send it to the server
// ip address and port
int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <file_to_send>\n";
        return 1;
    }
    const char* filename = argv[1];
    const char* server_ip = argv[2];
    const char* server_port = argv[3];

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(server_port));
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);

    //files want to be read a block at a time
    // sockets want a packet at a time
    // those two sizes AREN'T THE SAME!!!

    // read in something reasonable like 64k at a
    // time, send that. (UDP will do its best)

    // your code goes here...