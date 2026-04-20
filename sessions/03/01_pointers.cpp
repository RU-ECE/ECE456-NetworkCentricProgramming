#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
	auto x = 5;
	int* p = &x; // p is a pointer to an int, and it points to x
	cout << "x: " << x << endl; // prints 5
	cout << "*p: " << *p << endl; // prints 5, because *p is the value at the address p points to
	*p = 10; // changes the value at the address p points to (which is x) to 10
	cout << "x: " << x << endl; // prints 10

	int arr1[10];
	int n;
	cin >> n;
	int arr2[n]; // nonstandard (works on gcc)

	// old C way, used by Unix
	const auto heap = static_cast<int*>(malloc(sizeof(int) * 10)); // allocate memory for 10 ints
	free(heap);

	const auto q = new int[10]; // allocate memory for 10 ints, returns an int*
	delete[] q; // free the memory allocated by new

	return 0;
}
