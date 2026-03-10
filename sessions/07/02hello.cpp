#include <iostream>
#include <unistd.h>

using namespace std;
int main() {
    for (;;) { // while(true)
        cout << "hello";
        
        usleep(50'000);


    }

}