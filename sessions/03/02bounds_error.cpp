#include <iostream>

using namespace std;

int main() {
	auto n = 10;
	const auto p = new int[n]; // allocate memory for 10 ints

	for (; n < 1'000'000; n *= 10) {
		cout << n << endl;
		for (auto i = 0; i <= n; i++) // out of bounds (crash)
			p[i] = i; // fill the array with values
	}
}
