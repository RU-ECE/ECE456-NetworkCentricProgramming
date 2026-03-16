#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
using namespace std;
int main() {
    int status =mkdir("xyz", 0755);
    cout << "status: " << status << endl;
    return 0;
}
