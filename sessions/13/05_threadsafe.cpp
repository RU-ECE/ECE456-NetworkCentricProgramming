#include <iostream>
#include <thread>
#include <vector>

using namespace std;

vector<int> v;

void f() {
    for (int i = 0; i < 1000; i++)
        v.push_back(i);
}

int main() {
    thread t1(f);
    thread t2(f);
    t1.join();
    t2.join();
    cout << v.size() << endl;
    return 0;
}