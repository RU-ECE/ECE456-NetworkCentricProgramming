#include <unistd.h>


/*
    THis example writes bytes really slowly.
    Why?

    1. size of the buffer should be an integer multiple of block size
    find block size with stat -f /your/path

    2. the bigger the buffer, the fewer writes
      each write takes time to go into the OS
      

*/

void write_file_slow(int fh, uint64_t num_bytes) {
  uint32_t buf[500] = {0};
  for (uint64_t i = 0; i < num_bytes; i += sizeof(buf)) {
    write(fh, buf, size(uint32_t));
  }
}

int main(int argc, char *argv[]) {
    //                            rwx r-- r--
    // leading zero is OCTAL      110 100 100
    int fh = creat("output.txt", 0644);
    write_file_slow(fh, 1024*1024*1024);
    close(fh);
    return 0;
}