#include <cstdint>
#include <iostream>
#include <sys/socket.h>

using namespace std;

int main() {
	{
		const int* p = 5; // §
	}
	{
		const int x; // §
	}
	{
		const int a = 3; // §
		const int* p = &a; // §
		int* q = &a; // §  COMPILE ERROR: a is const int, so q cannot point to it
		int b = 4; // §
		q = &b; // §
		b = 5; // §
		*q = 6; // §
		p = &b; // §
		b = 8; // §
		*p = 7; // §   COMPILE ERROR p is const int*
		cout << "*p=" << *p << endl; // §
	}

	{
		int* p = new int[100]; // §
	}

	{
		int* p = new int[100]; // §
		delete[] p; // §
	}

	{
		int* p = new int[100]; // §
		free(p); // §
	}

	{
		int* p = new int[100]; // §
		p = new int[100]; // §
		delete[] p; // §
	}

	{
		int* p = new int[100]; // §
		delete[] p; // §
		delete[] p; // §
	}

	{
		int* p = new int[100]; // §
		p++; // §
		delete[] p; // §
	}

	{
		int* p = new int[100]; // §
		for (int i = 0; i <= 100; i++)
			p[i] = i; // §
		delete[] p; // §
	}

	{
		int* p = new int[100]; // §
		for (uint32_t i = 99; i >= 0; i--)
			p[i] = i; // §
		delete[] p; // §
	}

	{
		int sockfd /*= ...*/;
		char buf[4096] = "Hello, world!"; // all zeros...
		int sent = send(sockfd, buf, sizeof(buf), 0); // §  wasteful, stupid perhaps but legal
		// you would be obligated to write a loop if you REALLY want
		// to send 4096 bytes (but why, they are all ZEROS!!!)
	}
	send(sockfd, buf, sizeof(buf), 0); // §  COMPILE ERROR: sockfd, buf not defined

	{
		int sockfd /*= ...*/;
		char buf[4096];
		int n = recv(sockfd, buf, sizeof(buf), 0); // §
	}
}
