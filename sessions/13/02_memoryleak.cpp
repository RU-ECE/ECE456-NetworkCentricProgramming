#include <iostream>

// this function allocates memory. You must remember to delete the memory later
int* f(int n) {
	int* p = new int[n];
	return p;
}


int main() {
	int* p = f(10);
	// remember to delete or leak it!
}
