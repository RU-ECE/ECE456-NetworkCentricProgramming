#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
	const int status = chdir("xyz");
	cout << "status: " << status << endl;
	return 0;
}
