#include <unistd.h>
#include <iostream>

int main(int argc, char** argv) {

	char tmp[256];
	getcwd(tmp, 256);

	std::cout << "Before: " << tmp << std::endl;

	chdir(argv[1]); // p.184

	getcwd(tmp, 256);
	std::cout << "After: " << tmp << std::endl;
}