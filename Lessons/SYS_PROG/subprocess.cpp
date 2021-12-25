#include <stdlib.h>

int main(int argc, char** argv) {

    return system(argv[1]);
    //Это пара fork() + exec() - об этом потом
}