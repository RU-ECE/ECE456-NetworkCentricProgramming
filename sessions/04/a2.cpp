#include <iostream>
using namespace std;
// on this side, the compiler
// thinks it's float
float a = 3.0;

extern "C" {
    // the compiler doesn't catch this 
    // because it is C
  void f(int x);
}

int main() {
  f(2);
  cout << a << endl; // 3
}