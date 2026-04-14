#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

void myfunc(const int signum) { cout << "signal " << signum << " received" << endl; }

int main() {
	signal(SIGINT, myfunc);
	while (true) {
		cout << "waiting for signal" << endl;
		sleep(1);
	}
}
