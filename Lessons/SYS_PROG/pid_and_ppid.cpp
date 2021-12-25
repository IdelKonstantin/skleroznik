#include <iostream>

#include <sys/types.h>
#include <unistd.h>

/* id && top */

int main() {

    std::cout << "Привет из професса с PID = " 
    << getpid() << "\nИдентификатор родительского процесса PPID = " 
    << getppid() << "\nИдентификатор группы GID = " 
    << getgid() << "\nИдентификатор пользователя UID = " 
    << getuid() << std::endl;

    while(1); 
}