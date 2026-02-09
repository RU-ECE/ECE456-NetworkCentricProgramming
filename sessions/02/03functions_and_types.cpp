#include <cstdint> // portable integer types
#include <iostream>

using namespace std;

void f() { cout << "Hello from f()" << endl; }

uint32_t add(uint32_t a, uint32_t b) { return a + b; }

uint32_t mul(uint32_t a, uint32_t b) { return a * b; }

// 64-bit functions are different
uint64_t add(uint64_t a, uint64_t b) { return a + b; }

uint64_t mul(uint64_t a, uint64_t b) { return a * b; }

// 64-bit functions are different
float add(float a, float b) { return a + b; }

float mul(float a, float b) { return a * b; }

void g(int x) { cout << "Hello x=" << x << " from g(), x = " << x << endl; }

int main() {
	f();
	g(42);
	uint32_t a = 5, b = 6;
	uint32_t c = add(a, b); // Z_3addjj
	uint32_t d = mul(a, b); // Z_3mul

	uint64_t e = add(a, b); // Z_3addjj
	uint64_t x = 5LL, y = 6LL;
	uint64_t f = add(a, b); // Z_3addyy
	for (auto i = 0; i < 2'000'000'000; i++) {
	}
	return 0;
}
