#include <iostream>
using namespace std;
int a = 3;

extern "C" {
  void f();
}

int main() {
  f();
  cout << a << endl; // 3
}