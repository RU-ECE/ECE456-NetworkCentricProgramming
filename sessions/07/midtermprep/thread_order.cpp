#include <iostream>
#include <thread>
using namespace std;

void f1() {
    cout << "1\n";
}

void f2() {
    cout << "2\n";
}

int main() {

    thread t1(f1);
    thread t2(f2);

    cout << "3\n";

    t1.join();
    t2.join();
}