#include <iostream>

using namespace std;

constexpr int n = 1'000'000'000;
int main() {
    for (int i = 0; i < 100; i++) {
        int* p = new int[n];
        for (int i = 0; i < n; i++) {
            p[i] = i;
        }
        // we leak the memory and try to take me down
    }

}