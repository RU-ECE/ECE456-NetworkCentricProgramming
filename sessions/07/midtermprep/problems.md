```cpp

int main() {
  fork();
  fork();
  fork();
}
```
// how many tasks run?


```cpp

int main() {
  for (int i = 0; i < 100; i++)
    fork();  // 2^100 processes? Even if we kill them immediately, it takes time
}
```