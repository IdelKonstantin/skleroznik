#include <unistd.h>
#include <iostream>

extern char** environ;

int main(int argc, char** argv) { // Перечень П/О Т.7.1 стр 267

	for(int i = 0; *(environ+i) != NULL; ++i) {

    	fprintf(stderr, "%s\n", *(environ+i));    
    }
}