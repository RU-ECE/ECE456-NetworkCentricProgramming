#include <chrono>
#include <iostream>

using namespace std;

void f(const uint64_t n) {
	for (uint64_t i = 0; i < n; ++i) {
		// Simulate some work
		volatile uint64_t temp = i * i;
	}
}

int main() {
	constexpr uint64_t iterations = 100'000'000;
	const auto t0 = chrono::high_resolution_clock::now();
	f(iterations);
	const auto t1 = chrono::high_resolution_clock::now();
	const chrono::duration<double, milli> duration = t1 - t0;

	cout << "Iterations: " << iterations << endl;
	cout << "Execution time: " << duration.count() << " ms" << endl;

	return 0;
}
