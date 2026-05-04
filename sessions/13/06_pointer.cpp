#include <iostream>
using namespace std;

int* f(int n) {
    int* p = new int[n];
    for (int i = 0; i < n; i++) {
        p[i] = i;
    }
    return p;
}

int main() {
    int* p = f(-200);
    for (int i = 0; i < 10; i++) {
        cout << p[i] << endl;
    }
    delete[] p;
    return 0;
}