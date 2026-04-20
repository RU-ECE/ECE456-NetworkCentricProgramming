#include <iostream>
#include <thread>

using namespace std;

void f1() { cout << "1" << endl; }

void f2() { cout << "2" << endl; }

int main() {
	thread t1(f1);
	thread t2(f2);

	cout << "3" << endl;

	t1.join();
	t2.join();
}
