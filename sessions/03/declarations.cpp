int a;
int 
namespace ru {
    int a; // ru::a
}

void f() {
    int a = 2;
    if (2 < 3) {
        int a = 3;
        cout << a;
    }
    {
        int a = 5;
        cout << a;
    }
    cout << a;
}

uint64_t sum(uint64_t n) {
    uint64_t s = 0;
    for (uint64_t i = 1; i <= n; ++i) {
        s += i*i;
    }
    return s;
}