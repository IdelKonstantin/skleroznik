#include <memory>
#include <iostream>

#define DEFER(func) std::shared_ptr<void> _(nullptr,[](...){func;})

void seySomething(int) {
    
    std::cout << " world!!!" << std::endl;
}

int main() {
    
    DEFER(seySomething(42));
    std::cout << "Hello";
}
