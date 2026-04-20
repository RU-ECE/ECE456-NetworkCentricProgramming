#include <vector>
using namespace std;

inline void doubleme(const int x, vector<int>& v) {
	const int n = v.size();
	for (auto i = 0; i < n; i++)
		v.push_back(i);
	v.push_back(x); // stick x at the end
}

int main() {
	vector v = {1, 2, 3, 4, 5};
	doubleme(3, v); // not helpful
}
