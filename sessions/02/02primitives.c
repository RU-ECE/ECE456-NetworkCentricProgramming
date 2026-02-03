#include <stdio.h>

//legacy c data types
int main() {
  char a = 'A';          // character 1 fundamental unit of memory
  int b = 100;          // integer on most "normal" desktop
  // int might be signed/unsigned >= 16bits
  // int >= short int
  short int c = 32767;
  short c2;
  long int d = 1000000000L; // long integer >= 32bits
  for (int i = 0; i < 100000; i++) { // will fail on Arduino
    printf("%d, i");
  }
//  sizeof(char)  is always 1 meaningless

  float f = 3.14f;      // single precision floating point
  double g = 3.141592653589793; // double precision floating point
  long double h = 3.14159265358979323846L; // extended precision floating point  
}