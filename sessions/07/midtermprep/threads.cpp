#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;

void f(const char msg[], uint64_t delay, uint64_t count) {
    for (int i = 0; i < count; i++) {
        cout << msg << endl;
        usleep(delay);
    }
    cout << msg << endl;
}

int main() {
    thread t1(f, "H", 100000, 150);
    thread t2(f, "W", 200000, 100);
    t1.join();
    t2.join();
    // approximately how long will this program take to run (round off to nearest 0.1 seconds)?
    // How many times will the word "Hello" be printed?
    // How many times will the word "World" be printed?
    // in what order will they be printed?
    // are the words guaranteed to be on a single line each?
    return 0;
}