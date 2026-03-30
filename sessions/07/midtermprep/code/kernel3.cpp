#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;
int main() {
    char filename[10] = "x";
    for (int i = 1; i <= 3; i++) {
      filename[1] = '0' + i;
      filename[2] = '\0';
      int status = mkdir(filename, 0755);
      cout << "status: " << status << endl;
      status = chdir(filename);
      cout << "status: " << status << endl;
      ofstream f("yo!");
      f << "hello" << endl;
      f.close();
      status = chdir("..");
      cout << "status: " << status << endl;
    }
  return 0;
}
