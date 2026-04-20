#include <cstdint>
#include <iostream>

using namespace std;

uint64_t sum(const uint32_t n) {
	uint64_t s = 0;
	for (uint32_t i = 1; i <= n; i++)
		s += i;
	return s;
}

int main() {
	long long sum = 0;
	for (long long i = 0; i < 1'000'000LL; i++)
		sum += i;
	cout << "sum=" << sum << endl;
	return 0;
}
