#include <iostream>

void foo(int* ptr) {

	ptr = new int[100];
}

void bar(int* ptr) {

	delete[](ptr);
	ptr = nullptr;
}

void baz(int* ptr) {

	foo(ptr);
	bar(ptr);
	std::cout << *ptr << std::endl;
}

int main (int argc, char** argv) {

	int* ptr;
	baz(ptr);
}