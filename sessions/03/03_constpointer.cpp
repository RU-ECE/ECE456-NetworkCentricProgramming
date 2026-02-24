
void f() {
    int*p = new int[100];
    // leak
}

int main() {
    int a = 4;
    const int b = 5;
    const int* c = &a;
    a = 19;
    const int* d = &b;
    //    *d = 20; // compile error, not allowed to write
//    int* p = &b; // error p is not const
    int*p = (int*)&b;
    *p = 20; // now this is allowed (BAD)
    for (int i = 0; i < 100'000'000; i++) {
        f();
    } // google leak detector g++ -fsanitize=leak -g badinline.cpp
    // valgrind
}