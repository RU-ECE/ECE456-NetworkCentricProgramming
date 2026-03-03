#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]) { // ./a.out "hello world"
	const char* msg = argv[1];
	const int fd = open("test.txt", O_RDWR | O_CREAT, 0666);
	if (fd < 0) {
		cout << "Error opening file!" << endl;
		return 1;
	}
	write(fd, msg, strlen(msg));
	close(fd);
}
