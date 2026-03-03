#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <thread>

using namespace std;

int thread_counter = 0;  // shared among threads

void thread_func() {
    for (int i = 0; i < 5; ++i) thread_counter++;
    cout << "Thread done, counter=" << thread_counter << "\n";
}

int fork_counter = 0;    // parent and child have separate copies

void thread_example() {
    thread t(thread_func);
    t.join();
    cout << "Parent thread sees counter=" << thread_counter << "\n";
}

void fork_example() {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed\n";
        return;
    }
    else if (pid == 0) {
        // Child process
        for (int i = 0; i < 5; ++i) fork_counter++;
        cout << "Child done, counter=" << fork_counter << "\n";
    } else {
        // Parent process
        wait(nullptr);  // wait for child to finish
        cout << "Parent sees counter=" << fork_counter << "\n";
    }
}

int main() {
    cout << "=== Thread Example ===\n";
    thread_example();

    cout << "\n=== Fork Example ===\n";
    fork_example();

    return 0;
}