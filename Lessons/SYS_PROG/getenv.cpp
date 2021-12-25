#include <unistd.h>
#include <iostream>

int main(int argc, char** argv) {

    std:: cout << getenv(argv[1]) << std::endl;

    //Что бы задать в коде П/О - setenv()
}