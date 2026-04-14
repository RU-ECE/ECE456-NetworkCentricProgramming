#include <stdio.h>

int main() {
	int a, b;
	scanf("%d %d", &a, &b);
	const int c = a + b;
	printf("%d\n", c);
	return 0;
}
