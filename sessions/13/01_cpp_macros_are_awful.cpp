#include <iostream>

#define max(a, b) (a > b ? a : b)
//      max(3+2, 6-1) 3+2 > 6-1 ? 3+2 : 6-1
int main() {
    int a = 10;
    int b = 20;
    std::cout << max(a, b) << std::endl;
    std::cout << max("hello", "yo") << std::endl;
    std::cout << max(3+2, 7-1) << std::endl;
    //std::cout << max(a++, b++) << std::endl;  
    return 0;
}