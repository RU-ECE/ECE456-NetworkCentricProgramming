#include <iostream>
#include <unistd.h>

using namespace std;

constexpr int n = 1'000'000'000;
int main() {
	for (int i = 0; i < 10; i++) {
		const int pid = fork();
		if (pid < 0) {
			cerr << "Fork failed" << endl;
			return 1;
		}
		if (pid == 0) { // only the children steal memory
			cout << "Child process" << endl;
			const auto p = new int[n];
			for (int j = 0; j < n; j++)
				p[j] = j;
			// it's dead jim
		}
	}
}
