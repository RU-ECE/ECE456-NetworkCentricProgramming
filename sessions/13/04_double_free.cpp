#include <iostream>

int* f(int n) {
    int* p = new int[n];
    return p;
}

int main() {
    int* p = f(10);
    delete [] p;
    delete[] p;
    return 0;
}