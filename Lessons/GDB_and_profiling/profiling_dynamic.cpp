#include <memory>

void foo() {

	//Bad
	int* ptr = new int[10000];

	//Good
	//std::unique_ptr<int[]> ptr{new int[10000]};
}

int main () {

	foo();
}
