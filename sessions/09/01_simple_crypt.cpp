#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		return 1;
	}
	const char key = argv[1][0]; // Get the first character of the key from the command line argument
	const char* filename = argv[2];
	// 'a' --> 'm', 'b' --> 'n', ..., 'z' --> 'l'
	// 'A' --> 'M', 'B' --> 'N', ..., 'Z' --> 'L'
	// XOR each letter with the secret key
	ifstream input_file(filename);
	char buf[1024];
	while (input_file.get(buf, sizeof(buf))) {
		for (int i = 0; i < 1024; i++) {
			if (buf[i] == '\0')
				break;
			cout << hex << (buf[i] ^ key) << ' '; // Stop at the end of the string
		}
		cout << endl;
	}
}
