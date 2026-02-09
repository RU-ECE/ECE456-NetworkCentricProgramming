#include <cstdint>
#include <fcntl.h>
#include <unistd.h>


/**
 * This example writes bytes really slowly.
 *
 * Why?
 *
 * 1. The size of the buffer should be an integer multiple of block size; find block size with stat -f /your/path
 *
 * 2. The bigger the buffer, the fewer the writes, as each write takes time to go into the OS
 */

void write_file_slow(int fh, uint64_t num_bytes) {
	uint32_t buf[500] = {};
	for (uint64_t i = 0; i < num_bytes; i += sizeof(buf))
		write(fh, buf, sizeof(buf));
}

int main(int argc, char* argv[]) {
	//                            rwx r-- r--
	// leading zero is OCTAL      110 100 100
	int fh = creat("output.txt", 0644);
	write_file_slow(fh, 1024 * 1024 * 1024);
	close(fh);
	return 0;
}
