#include <iostream>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

using namespace std;

int thread_counter = 0; // shared among threads

void thread_func() {
	for (int i = 0; i < 5; ++i)
		thread_counter++;
	cout << "Thread done, counter=" << thread_counter << endl;
}

int fork_counter = 0; // parent and child have separate copies

void thread_example() {
	thread t(thread_func);
	t.join();
	cout << "Parent thread sees counter=" << thread_counter << endl;
}

void fork_example() {
	const pid_t pid = fork();

	if (pid < 0) {
		cerr << "Fork failed" << endl;
		return;
	} else if (pid == 0) {
		// Child process
		for (int i = 0; i < 5; ++i)
			fork_counter++;
		cout << "Child done, counter=" << fork_counter << endl;
	} else {
		// Parent process
		wait(nullptr); // wait for child to finish
		cout << "Parent sees counter=" << fork_counter << endl;
	}
}

int main() {
	cout << "=== Thread Example ===" << endl;
	thread_example();

	cout << endl << "=== Fork Example ===" << endl;
	fork_example();

	return 0;
}
