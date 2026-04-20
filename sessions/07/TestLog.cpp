#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <semaphore.h>
#include <string>
#include <sys/mman.h>
#include <unistd.h>

#include "SharedLogBuffer.hpp"

using namespace std;

int main(int argc, char** argv) {
	auto shm_name = "/logclient_shm";
	auto sem_name = "/logclient_sem";

	// open/create shared memory
	const int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
	ftruncate(fd, sizeof(SharedLogBuffer));
	auto* buffer = static_cast<SharedLogBuffer*>(
		mmap(nullptr, sizeof(SharedLogBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

	// open/create semaphore
	sem_t* sem = sem_open(sem_name, O_CREAT, 0666, 0);
	if (!sem) {
		perror("sem_open");
		return 1;
	}

	// produce some log messages
	for (int i = 0; i < 100; ++i) {
		const size_t head = buffer->head.load(memory_order_relaxed);
		LogMessage& msg = buffer->messages[head % LOG_BUFFER_SIZE];

		string s = "Test log message " + to_string(i);
		msg.length = s.size();
		memcpy(msg.text, s.data(), msg.length);

		buffer->head.store(head + 1, memory_order_release);
		sem_post(sem); // notify logger
		usleep(10'000); // simulate work
	}

	cout << "Client finished producing messages" << endl;
	return 0;
}
