#include <iostream>
#include <mutex>
#include <thread>

using namespace std;
mutex m;

uint64_t balance = 0;

constexpr uint64_t n = 500'000'000;

void deposit() {
	for (int i = 0; i < n; i++)
		balance += 1;
}

void withdraw() {
	for (int i = 0; i < n; i++)
		balance -= 1;
}

int main() {
	thread t1(deposit);
	thread t2(withdraw);
	t1.join();
	t2.join();
	cout << "Balance: " << balance << endl;
	return 0;
}
