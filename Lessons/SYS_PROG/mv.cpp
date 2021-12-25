#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv) {

	const char* oldfilename = argv[1];
	const char* newfilename = argv[2];

	rename(oldfilename, newfilename);//p.166
}