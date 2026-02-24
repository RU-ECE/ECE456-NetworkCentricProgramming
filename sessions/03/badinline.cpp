#include <vector>
using namespace std;

inline void doubleme(int x, std::vector<int>& v) {
    int n = v.size();
    for (int i = 0; i < n; i++) {
        v.push_back(i);
    }
    v.push_back(x); // stick x at the end
}

int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    pitythefool(3, v); // not helpful
}