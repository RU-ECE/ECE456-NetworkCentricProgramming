#include <cstdio>

inline int add(const int a, const int b) { return a + b; }

int main() {
	const int a = 1;
	const int b = 2;
	const int c = add(a, b); // 1 + 2 = 3

	printf("%d\n", c);
	return 0;
}
