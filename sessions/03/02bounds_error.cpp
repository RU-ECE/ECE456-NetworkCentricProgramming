#include <iostream>
using namespace std;

int main() {
    int n = 10;
    int* p = new int[n]; // allocate memory for 10 ints

    for (; n < 1000000; n *= 10) {
        cout << n << endl;
        for (int i = 0; i <= n; i++) { // out of bounds (crash)
            p[i] = i; // fill the array with values
        }
}
}