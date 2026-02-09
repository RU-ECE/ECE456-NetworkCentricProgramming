#include <stdio.h>

// legacy C data types
int main() {
	auto a = 'A'; // character 1 fundamental unit of memory
	auto b = 100; // integer on most "normal" desktop
	// int might be signed/unsigned >= 16bits
	// int >= short int
	short int c = 32'767;
	short c2;
	auto d = 1'000'000'000L; // long integer >= 32bits
	for (auto i = 0; i < 100'000; i++) // will fail on Arduino
		printf("%d", i);
	sizeof(char); // is always 1; meaningless

	auto f = 3.14f; // single precision floating point
	auto g = 3.141592653589793; // double precision floating point
	auto h = 3.14159265358979323846L; // extended precision floating point
}
