#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;

void f(uint64_t delay) {
    for (int i = 0; i < 10; i++) {
        cout << "Hello " << i << endl;
        usleep(delay);
    }
}

int main() {
    thread t1(f, 1000000); // 100k uSec
    thread t2(f, 2000000); // 200k uSec
    t1.join();
    t2.join();
    return 0;
}
