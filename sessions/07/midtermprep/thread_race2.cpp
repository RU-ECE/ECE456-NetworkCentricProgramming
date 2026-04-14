#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

int counter = 0;

void work() {
	for (int i = 0; i < 100'000; i++)
		counter++;
}

void work2() {
	for (int i = 0; i < 100'000; i++) {
		counter++;
		usleep(1);
	}
}

int main() {
	thread t1(work);
	thread t2(work);

	t1.join();
	t2.join();

	cout << counter << endl;
}
