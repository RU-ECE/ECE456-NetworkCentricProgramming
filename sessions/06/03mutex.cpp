#include <thread>
#include <mutex>
#include <iostream>
#include <unistd.h>

using namespace std;
mutex m;

uint64_t balance = 0;

constexpr uint64_t n = 10'000'000;

void deposit() {
    for (int i = 0; i < n; i++) {
        m.lock();
        balance += 1;
        m.unlock();
    }
}

void withdraw() {
    for (int i = 0; i < n; i++) {
        m.lock();
        balance -= 1;
        m.unlock();
    }
}


int main() {
    thread t1(deposit);
    thread t2(withdraw);
    t1.join();
    t2.join();
    cout << "Balance: " << balance << endl;
    return 0;
}
