#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv) {

    auto fd = open(argv[1], O_RDONLY);

    char buf[0xFF];
    read(fd, buf, 0xFF); //write (+ pread, pwrite -> атомарные синхронизируемые аналоги)
    printf("%s", buf);

    close(fd);

    //Если нужна запись, то используем fsync
    //Если надо изменить свойства открытого файла, то fcntl...
}