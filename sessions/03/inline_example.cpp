#include <cstdint>
#include <iostream>

using namespace std;

inline uint64_t add(const uint64_t a, const uint64_t b) { return a + b; }

uint64_t sub(const uint64_t a, const uint64_t b) { return a - b; }

int main() {
	uint64_t a = 1, b = 2;
	const uint64_t c = add(1, 2); // generates 3 (no code!)
	const uint64_t d = add(a, b); // generates 3 (a and b are known!)
	const uint64_t e = sub(a, b); // generates call because compiler does not know what sub does
	cin >> a >> b; // now a and b are not known at compile time
	const uint64_t f = add(a, b); // generates code (but not call, there is no function)
	cout << "c: " << c << endl;
	cout << "d: " << d << endl;
	cout << "e: " << e << endl;
	cout << "f: " << f << endl;
	return 0;
}
