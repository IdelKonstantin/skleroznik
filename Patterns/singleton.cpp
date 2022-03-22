#include <iostream>

class foo {
    
    public:
    
        static foo& getInstance() {
            
            static foo f;
            return f;
        }
        
        void sayHello() {
            
            std::cout << "Hello World" << std::endl;
        }
        
        ~foo() {}
    
    private:
    
        foo() {}
};


int main()
{
    foo::getInstance().sayHello();
    return 0;
}