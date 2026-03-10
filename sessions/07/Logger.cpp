#include "SharedLogBuffer.hpp"
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <csignal>
#include <atomic>

using namespace std;

atomic<bool> flush_now{false};
atomic<bool> terminate_now{false};

void sig_flush(int) {
    flush_now = true;
    cerr << "flushing!\n";
}

void sig_terminate(int) {
    terminate_now = true;
    cerr << "I'll be back!\n";
}

int main(int argc, char* argv[]) {
    signal(SIGUSR1, sig_flush);   // flush on SIGUSR1
    signal(SIGINT, sig_terminate); // terminate on Ctrl+C
    ofstream log("log.txt", ios::out | ios::app);

     // create or open shared memory
    int fd = shm_open(shm_name, O_RDWR | O_CREAT, 0666);
    if (fd < 0) { perror("shm_open"); return 1; }
    if (ftruncate(fd, sizeof(SharedLogBuffer)) != 0) { perror("ftruncate"); return 1; }

    // semaphore controls shared access to the data structure
    // only one party can write at a time
    sem_t* sem = sem_open(sem_name, O_CREAT, 0666, 0);
    if (!sem) { perror("sem_open"); return 1; }

    auto* buffer = (SharedLogBuffer*)mmap(nullptr, sizeof(SharedLogBuffer),
                                          PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buffer == MAP_FAILED) { perror("mmap"); return 1; }

    buffer->head = 0;
    buffer->tail = 0;


 
    while(true) {
        if (flush_now) {
            log << "flushing...\n";
            log.flush();
            flush_now = false;
        }
        if (terminate_now)
            break;

        // wait for messages, but timeout periodically to check signals
        timespec ts{0, 10000000}; // 10ms
        while (sem_timedwait(sem, &ts) == -1) break; // ignore timeout

        size_t tail = buffer->tail.load(memory_order_relaxed);
        size_t head = buffer->head.load(memory_order_acquire);

        while (tail < head) {
            LogMessage& msg = buffer->messages[tail % LOG_BUFFER_SIZE];
            log.write(msg.text, msg.length);
            log.put('\n');
            cout.write(msg.text, msg.length);
            cout.put('\n');
            tail++;
        }
        buffer->tail.store(tail, memory_order_release);
        if (flush_now) {
            log.flush();
            cout << "[Logger flushed]\n";
            flush_now = false;
        }        
    }
    log.flush();
    cout << "Logger terminating.\n";
    return 0;
}