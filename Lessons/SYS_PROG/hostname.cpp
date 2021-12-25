#include <iostream>
#include <unistd.h>

int main() {
    char tmp[64];
    gethostname(tmp, 64);
    std::cout << tmp << std::endl;
}