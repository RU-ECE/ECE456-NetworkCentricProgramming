#include "SharedLogBuffer.hpp"
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    const char* shm_name = "/logclient_shm";
    const char* sem_name = "/logclient_sem";

    // open/create shared memory
    int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(SharedLogBuffer));
    auto* buffer = (SharedLogBuffer*)mmap(nullptr, sizeof(SharedLogBuffer),
        PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // open/create semaphore
    sem_t* sem = sem_open(sem_name, O_CREAT, 0666, 0);
    if (!sem) { perror("sem_open"); return 1; }

        // produce some log messages
    for (int i = 0; i < 100; ++i) {
        size_t head = buffer->head.load(std::memory_order_relaxed);
        LogMessage& msg = buffer->messages[head % LOG_BUFFER_SIZE];

        std::string s = "Test log message " + std::to_string(i);
        msg.length = s.size();
        std::memcpy(msg.text, s.data(), msg.length);

        buffer->head.store(head + 1, std::memory_order_release);
        sem_post(sem); // notify logger
        usleep(10000); // simulate work
    }

    std::cout << "Client finished producing messages\n";
    return 0;
}