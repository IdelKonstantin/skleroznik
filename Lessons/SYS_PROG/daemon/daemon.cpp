#include <iostream>
#include <unistd.h>

int main() {

	while(1) {

		std::cout << "Hello from daemon..." << std::endl;
		sleep(5);
	}
}