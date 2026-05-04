#include <iostream>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

int openfile(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fc < 0)
      return -1;
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }
    return file;
}

// hmm. this sort of works because their function checks if the file is bad?


int main() {
    int fd = openfile("example.txt");
#if 0
// suppose we forget to check....
    if (fd < 0) {
        cout << "Failed to open file" << endl;
        return 1;
    }
#endif
    char buffer[4096]
    int n = read(fd, buffer, 4096);
    while (n > 0) {
        write(1, buffer, n);
        n = read(fd, buffer, 4096);
    }
    close(fd);
    return 0;
}
