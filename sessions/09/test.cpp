#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return 1;
	}
	const char* filename = argv[1];
}
