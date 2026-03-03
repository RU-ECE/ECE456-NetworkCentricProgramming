#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    }
    else if (pid == 0) {
        cout << "Child process" << endl;
    }
    else {
        cout << "Parent process" << endl;
    }
    cout << "Hello, World!" << endl;
    return 0;
}