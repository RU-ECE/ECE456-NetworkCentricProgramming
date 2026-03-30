#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
using namespace std;
int main() {
    int status =chdir("xyz");
    cout << "status: " << status << endl;
    return 0;
}
