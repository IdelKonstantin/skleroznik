#include <iostream>
#include <unistd.h>

int main() {
    char tmp[256];
    getcwd(tmp, 256);
    std::cout << tmp << std::endl;
}