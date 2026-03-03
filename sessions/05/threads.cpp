#include <condition_variable>
#include <iostream>
#include <thread>

void f(const char* msg, const uint32_t repeat) {
	for (uint32_t i = 0; i < repeat; i++) {
		std::cout << msg << std::endl;
		usleep(100'000);
	}
}

int main() {
	std::thread t1(f, "Hello", 10);
	std::thread t2(f, "World", 20);
	t1.join();
	t2.join();
	return 0;
}
