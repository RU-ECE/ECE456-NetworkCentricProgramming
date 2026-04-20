#include <cstdint>
#include <iostream>

using namespace std;

int a;
namespace ru {
	int a; // ru::a
}

void f() {
	constexpr auto a = 2;
	if constexpr (2 < 3) {
		const auto a = 3;
		cout << a;
	}
	{
		const auto a = 5;
		cout << a;
	}
	cout << a;
}

uint64_t sum(const uint64_t n) {
	uint64_t s = 0;
	for (uint64_t i = 1; i <= n; ++i)
		s += i * i;
	return s;
}
