#include <chrono>
#include <cstdint>
#include <iostream>

using namespace std;

/**
 * @file primes.cpp
 * @brief Starter code for prime counting assignment.
 *
 * Fill in the required functions without changing the provided signatures.
 * Do not add optimizations beyond the assignment instructions unless asked.
 *
 * @author _______
 * @date _______
 * @course ECE 451
 * @assignment _______
 */

/**
 * @brief Determine whether a number is prime.
 *
 * Return true if `n` is prime, false otherwise. The required approach for
 * this assignment is to test divisibility by every integer from 2 to n-1.
 *
 * Example for n = 17:
 * 17 % 2 != 0, 17 % 3 != 0, 17 % 4 != 0, ..., 17 % 16 != 0
 * Therefore 17 is prime.
 *
 * @param n Unsigned 64-bit integer to test.
 * @return true if `n` is prime; false otherwise.
 */
bool is_prime(uint64_t n) { return false; }

/**
 * @brief Count the primes in an inclusive range.
 *
 * Calculate the number of prime values from `a` to `b`, inclusive. The
 * required approach is to repeatedly call `is_prime` for every integer in
 * the range.
 *
 * @param a Start of range (inclusive).
 * @param b End of range (inclusive).
 * @return Number of primes in the range [a, b].
 */
uint64_t count_primes(uint64_t a, uint64_t b) { return 0; }

int main() {
	cout << count_primes(1, 1000) << endl; // 168?
	const auto before = chrono::high_resolution_clock::now();
	const auto count = count_primes(1, 1'000'000);
	const auto after = chrono::high_resolution_clock::now();
	cout << count << " in " << chrono::duration_cast<chrono::microseconds>(after - before).count() << " microseconds"
		 << endl;
	return 0;
}
