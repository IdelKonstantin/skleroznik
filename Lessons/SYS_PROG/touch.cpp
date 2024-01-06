#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv) {

    auto fd = fopen("2.txt", "a");
    char msg[256];
    scanf("%s", msg);
    fprintf(fd, "%s", msg);
    fclose(fd);
}