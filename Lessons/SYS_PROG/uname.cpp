#include <iostream>

#include <sys/utsname.h>

int main()
{
    struct utsname name;
    
    uname(&name);
    
    std::cout 
    << name.sysname << " " 
    << name.nodename << " " 
    << name.release << " " 
    << name.version << " " 
    << name.machine 
    << std::endl;
}
