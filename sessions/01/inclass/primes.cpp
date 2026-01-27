#include <iostream>
#include <cstdint>
#include <chrono>
using namespace std;
/*
  Author: _______


  return true if n is prime, false otherwise
  test by repeatedly testing if n is divisible by 
  every number from 2 to n-1.
  Example:
    17 %2 != 0, 17%3 != 0 17 % 4 != 0 ... 17 % 16 != 0
    therefore 17 is prime
*/

bool is_prime(uint64_t n) {

  return false;
}

/*
  Write a function that calculates the number of primes
  between a and b.
   repeatedly call is_prime for every number from a to b inclusive.
*/

uint64_t count_primes(uint64_t a, uint64_t b) {
  return 0;
}

int main() {
  cout << count_primes(1, 1000) << endl; //168?
  auto t0 = std::chrono::high_resolution_clock::now();
  uint64_t count = count_primes(1, 1'000'000);
  auto t1 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
  cout << count << " in " << duration << " microseconds" << endl;
  return 0;
}
 
