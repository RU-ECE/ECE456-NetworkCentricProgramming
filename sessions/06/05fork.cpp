#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
	const pid_t pid = fork();
	if (pid < 0) {
		cerr << "Fork failed" << endl;
		return 1;
	}
	cout << (pid == 0 ? "Child process" : "Parent process") << endl << "Hello, World!" << endl;
	return 0;
}
