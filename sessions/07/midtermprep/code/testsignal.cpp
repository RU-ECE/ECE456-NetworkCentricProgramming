#include <signal.h>
#include <iostream>
#include <unistd.h>
using namespace std;
void myfunc(int signum) {
    cout << "signal " << signum << " received" << endl;
}

int main() {
    signal(SIGINT, myfunc);
    while(true) {
        cout << "waiting for signal" << endl;
        sleep(1);
    }
}