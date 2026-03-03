#include <iostream>

using namespace std;

constexpr int n = 1'000'000'000;
int main() {
    for (int i = 0; i < 10; i++) {
        int pid = fork();
        if (pid < 0) {
            cerr << "Fork failed" << endl;
            return 1;
        }
        else if (pid == 0) { // only teh children steal memory
            cout << "Child process" << endl;
            int* p = new int[n];
            for (int i = 0; i < n; i++) {
                p[i] = i;
            }
            // it's dead jim
        }
        else {
    }

}