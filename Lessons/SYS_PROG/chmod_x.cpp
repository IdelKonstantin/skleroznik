#include <sys/stat.h>
#include <iostream>

int main(int argc, char** argv) {

	chmod(argv[1], S_IRWXU | S_IRWXG | S_IRWXO); // p.152 + chmod 600 filename
	//fchmod работает с фаловым дескриптором
}