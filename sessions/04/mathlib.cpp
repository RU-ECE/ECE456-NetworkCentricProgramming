#include "mathlib.hpp"

double choose(const int n, const int r) {
	if (r > n)
		return 0;
	if (r == 0 || r == n)
		return 1;
	double res = 1;
	for (auto i = 1; i <= r; i++)
		res *= (n - r + i) / static_cast<double>(i);
	return res;
}

double binomial(const int n, const int r) { return choose(n, r) / (factorial(r) * factorial(n - r)); }

double factorial(const int n) {
	if (n == 0)
		return 1;
	double res = 1;
	for (auto i = 1; i <= n; i++)
		res *= i;
	return res;
}
