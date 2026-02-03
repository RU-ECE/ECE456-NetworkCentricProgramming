#include <stdio.h>

int a; // global variable
int d = 4;
void f() {
    static int b; // static local variable
    static int c = 3;
    int e = 5;
    // stored in data segment
    printf("%d %d %d %d %d\n",a,b,c,d, e);
    b++;
    a++;
    c--;
    d--;
    printf("%d %d %d %d %d\n",a,b,c,d,e);
    e++;
}

int main() {
    int a,b; // auto (local) variables
    // on the stack or in registers
    f();
    f();
    printf("%d", a); // a is uninitialized
    scanf("%d %d",&a,&b);
    printf("%d %d",a-b,a+b);
    return 0;
}