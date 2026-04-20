#include <iostream>
#include <sys/stat.h>

using namespace std;

int main() {
	const int status = mkdir("xyz", 0755);
	cout << "status: " << status << endl;
	return 0;
}
