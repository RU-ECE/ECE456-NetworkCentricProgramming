#include <condition_variable>
#include <iostream>
#include <thread>

using namespace std;

void f(const char* msg, const uint32_t repeat) {
	for (uint32_t i = 0; i < repeat; i++) {
		cout << msg << endl;
		usleep(100'000);
	}
}

int main() {
	thread t1(f, "Hello", 10);
	thread t2(f, "World", 20);
	t1.join();
	t2.join();
	return 0;
}
