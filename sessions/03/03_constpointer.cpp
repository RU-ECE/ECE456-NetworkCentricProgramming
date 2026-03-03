void f() {
	auto p = new int[100];
	// leak
}

int main() {
	auto a = 4;
	const auto b = 5;
	const int* c = &a;
	a = 19;
	const int* d = &b;
	//    *d = 20; // compile error, not allowed to write
	//    int* p = &b; // error p is not const
	const auto p = (int*)&b;
	*p = 20; // now this is allowed (BAD)
	for (auto i = 0; i < 100'000'000; i++) {
		f();
	} // google leak detector g++ -fsanitize=leak -g badinline.cpp
	// valgrind
}
