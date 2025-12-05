#include <iostream>

namespace device {

    template<typename T>
    class Singleton {
        protected:
        
            Singleton() = default;
            ~Singleton() = default;
    
        public:
        
            Singleton(const Singleton&) = delete;
            Singleton& operator=(const Singleton&) = delete;
    
            static T& getInstance() {
                
                static T instance;
                return instance;
            }
    };

} // namespace device

/////////////////////////////////////////////////////////////////////////

class foo {
  
    public:
        foo() = default;
        void print() {
            std::cout << "Foooooo!!!!" << std::endl;
        }
};

/////////////////////////////////////////////////////////////////////////

int main()
{
    device::Singleton<foo>::getInstance().print();
    return 0;
}
