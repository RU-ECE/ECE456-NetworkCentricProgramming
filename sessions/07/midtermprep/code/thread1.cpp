#include <iostream>
#include <thread>
using namespace std;

void f() {
    cout << "hello" << endl;
}

void g() {
    cout << "world" << endl;
}

int main() {
    thread t1(f);
    thread t2(g);
    t1.join();
    t2.join();
    return 0;
}