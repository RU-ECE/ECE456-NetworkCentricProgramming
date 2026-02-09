#include <iostream>
#include <chrono>

using namespace std;

void f(uint64_t n) {
    for (uint64_t i = 0; i < n; ++i) {
        // Simulate some work
        volatile uint64_t temp = i * i;
    }
}

int main() {
    auto t0 = chrono::high_resolution_clock::now();
    f();
    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = t1 - t0;

    cout << "Sum: " << sum << endl;
    cout << "Execution time: " << duration.count() << " ms" << endl;

    return 0;
}