#include <unistd.h>

int main(int argc, char** argv) {

	unlink(argv[1]); // p.165
	//unlinkat работает с фаловым дескриптором
}