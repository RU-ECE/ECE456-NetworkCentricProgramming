#include <iostream>

using namespace std;

// this function allocates memory. You must remember to delete the memory later
vector<int> f(int n) {
	vector<int> v(n);
	for (int i = 0; i < n; i++)
		v[i] = i;
	return;
}


int main() {
	vector<int> v = f(10);
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << endl;
	return 0;
}
