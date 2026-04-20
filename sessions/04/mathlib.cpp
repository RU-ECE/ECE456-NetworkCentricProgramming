#include "mathlib.hpp"

double choose(const int b, const int r) {
	if (r > b)
		return 0;
	if (r == 0 || r == b)
		return 1;
	double res = 1;
	for (auto i = 1; i <= r; i++)
		res *= (b - r + i) / static_cast<double>(i);
	return res;
}

double binomial(const int b, const int r) { return choose(b, r) / (factorial(r) * factorial(b - r)); }

double factorial(const int b) {
	if (b == 0)
		return 1;
	double res = 1;
	for (auto i = 1; i <= b; i++)
		res *= i;
	return res;
}
