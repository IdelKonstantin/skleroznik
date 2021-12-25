#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv) {

    auto fd = open(argv[1], O_CREAT | O_TRUNC | S_IRWXU | S_IRWXG | S_IRWXO);
    close(fd);
}