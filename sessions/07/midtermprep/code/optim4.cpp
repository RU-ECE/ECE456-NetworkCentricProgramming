#include <cstdio>

inline int add(int a, int b) {
    return a + b;
}

int main() {
    int a = 1;
    int b = 2;
    int c = add(a, b); // 1 + 2 = 3

    printf("%d\n", c);
    return 0;
}