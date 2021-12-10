#include <iostream>

void foo() {

	int i;

	for(i; i < 5000; i++) {

		std::cout << i << std::endl;
	}
}

void bar() {

	int arr[3] = {1,2,3};

	std::cout << arr[4] << std::endl;
}

void bazz() {

	int* ptr = nullptr;
	std::cout << *ptr << std::endl;
}

int main () {

	foo();
	bar();
	bazz();
}
