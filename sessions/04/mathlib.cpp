#include "mathlib.hpp"

double choose(int n, int r) {
    if (r > n) return 0;
    if (r == 0 || r == n) return 1;
    double res = 1;
    for (int i = 1; i <= r; i++) {
        res *= (n - r + i) / (double)i;
    }
    return res;
}

double factorial(int n) {
    if (n == 0) return 1;
    double res = 1;
    for (int i = 1; i <= n; i++) {
        res *= i;
    }
    return res;
}

double binomial(int n, int r) {
    return choose(n, r) / (factorial(r) * factorial(n - r));
}