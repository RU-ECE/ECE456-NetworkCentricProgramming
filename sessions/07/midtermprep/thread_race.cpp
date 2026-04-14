#include <iostream>
#include <thread>

using namespace std;

int counter = 0;

void work() {
	for (int i = 0; i < 100'000; i++)
		counter++;
}

int main() {
	thread t1(work);
	thread t2(work);

	t1.join();
	t2.join();

	cout << counter << endl;
}
