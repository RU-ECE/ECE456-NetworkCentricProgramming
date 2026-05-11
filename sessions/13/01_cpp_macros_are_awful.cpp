#include <iostream>

using namespace std;

#define max(a, b) (a > b ? a : b)
//      max(3+2, 6-1) 3+2 > 6-1 ? 3+2 : 6-1
int main() {
	int a = 10;
	int b = 20;
	cout << max(a, b) << endl;
	cout << max("hello", "yo") << endl;
	cout << max(3 + 2, 7 - 1) << endl;
	// cout << max(a++, b++) << endl;
	return 0;
}
