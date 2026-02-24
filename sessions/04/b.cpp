#include <iostream>

using namespace std;

extern int a;
extern "C" {
void f() {
	cout << "a=" << a << endl; // 4
}
}
