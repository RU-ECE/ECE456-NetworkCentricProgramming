#include <iostream>

using namespace std;

constexpr int n = 1'000'000'000;
int main() {
	for (int i = 0; i < 100; i++) {
		const auto p = new int[n];
		for (int j = 0; j < n; j++)
			p[j] = j;
		// we leak the memory and try to take me down
	}
}
