#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
using namespace std;
int main() {
    int status = mkdir("testdir", 0777); // create a directory with read/write/execute permissions for everyone
    if (status < 0) {
        cerr << "Error creating directory" << endl;
        return 1;
    }
    chdir("testdir"); // change to the new directory
    int fh = creat("testfile.txt", 0666); // create a file with read/write permissions for everyone
    if (fh == -1) {
        cerr << "Error creating file" << endl;
        return 1;
    }
    write(fh, "Hello, kernel!\n", 15); // write to the file (kernel will handle this)
    close(fh);
    return 0;
}